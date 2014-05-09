#include "Puerta.h"

Puerta::Puerta()
{
    owner = "Puerta";
    in = new Queue<struct iMessage>(PATH, Q_PUERTA_FROM_INTERFACE, owner);
    in->get();
    out = new Queue<struct iMessage>(PATH, Q_PUERTA_TO_INTERFACE, owner);
    out->get();
    persona = new Queue<struct personMessage>(PATH, Q_PERSONA, owner);
    persona->get();
    salaAbajo = new Queue<struct personMessage>(PATH, Q_SALA_ABAJO, owner);
    salaAbajo->get();
    salaArriba = new Queue<struct personMessage>(PATH, Q_SALA_ARRIBA, owner);
    salaArriba->get();
    shm = new SharedMemory<struct registro>(PATH, SHM_CC_SALAS, owner);
    shm->get();
    registro = shm->attach();
    mutex = new Semaphore(PATH, SEM_MUTEX, owner);
    mutex->get();
    abajo = new Semaphore(PATH, SEM_ABAJO, owner);
    abajo->get();
    arriba = new Semaphore(PATH, SEM_ARRIBA, owner);
    arriba->get();
    llenaAbajo = new SemaphoreArray(PATH, SEM_FULL_BOTTOM, DOOR_AMOUNT, owner);
    llenaAbajo->get();
    llenaArriba = new SemaphoreArray(PATH, SEM_FULL_TOP, DOOR_AMOUNT, owner);
    llenaArriba->get();
    vacia = new SemaphoreArray(PATH, SEM_EMPTY, CC_AMOUNT, owner);
    vacia->get();
}

void Puerta::init()
{
    std::stringstream ss;
    struct iMessage msg;
    pid_t pid;
    while (true)
    {
        in->receive(&msg, 0);
        pid = fork();
        if (pid == 0)
        {
            int num = msg.sender - (msg.data.loc == ARRIBA ? M_PUERTA_ARRIBA : M_PUERTA_ABAJO);
            ss << "Componente " << owner << " recibi " << Helper::msgToString(msg.message) << " de puerta " << Helper::msgToString(msg.data.loc) << " " << num << " (" << msg.sender << ")" << std::endl;
            Helper::output(stdout, ss);
            switch (msg.message)
            {
                case ESPERAR_PERSONA_ABAJO:
                    msg.type = msg.sender;
                    msg.sender = M_PUERTA_ABAJO;
                    msg.message = ENTRO_PERSONA_ABAJO;
                    esperarPersona(ABAJO, msg.number);
                    break;
                case ESPERAR_PERSONA_ARRIBA:
                    msg.type = msg.sender;
                    msg.sender = M_PUERTA_ARRIBA;
                    msg.message = ENTRO_PERSONA_ARRIBA;
                    esperarPersona(ARRIBA, msg.number);
                    break;
                default:
                    ss << owner << " \033[41m\033[30mError\33[0m mensaje incorrecto" << Helper::msgToString(msg.message);
                    Helper::output(stderr, ss);
                    exit(EXIT_FAILURE);
            }
            out->send(&msg);
            ss << "Componente " << owner << " envie " << Helper::msgToString(msg.message) << std::endl;
            Helper::output(stdout, ss);
            exit(EXIT_SUCCESS);
        } else if (pid < 0)
        {
            perror("Sala: fork");
        }
    }
}

void Puerta::esperarPersona(enum location ubicacion, long numero)
{
    std::stringstream ss;
    struct personMessage msg;
    Semaphore * llena;
    Queue<struct personMessage> * myQueue;
    long myId;
    struct sala * sala;
    Semaphore * espera;
    if (ubicacion == ABAJO)
    {
        espera = abajo;
        sala = &registro->abajo;
        myQueue = salaAbajo;
        myId = M_PUERTA_ABAJO + numero;
        llena = new Semaphore(this->llenaAbajo, numero);
    } else
    {
        espera = arriba;
        sala = &registro->arriba;
        myQueue = salaArriba;
        myId = M_PUERTA_ARRIBA + numero;
        llena = new Semaphore(this->llenaArriba, numero);
    }
    ss << this->owner << " " << numero << " de " << Helper::msgToString(ubicacion) << " (" << myId << ")";
    std::string owner = ss.str();
    ss.str("");
    // Esperar a que alguien quiera entrar
    ss << owner << " esperando persona " << std::endl;
    Helper::output(stdout, ss);
    myQueue->receive(&msg, myId);
    ss << owner << " recibi: " << Helper::msgToString(msg.message) << " de " << msg.sender - M_PERS << " (" << (msg.sender) << ")" << std::endl;
    Helper::output(stdout, ss);
    if (msg.message != QUIERO_ENTRAR)
    {
        ss << owner << " \033[41m\033[30mError\33[0m mensaje incorrecto" << Helper::msgToString(msg.message);
        Helper::output(stderr, ss);
        exit(EXIT_FAILURE);
    }
    
    espera->wait();
    // O pongo esto dentro de la seccion critica,
    // o tengo que asegurarme de que haya solamente sola puerta haciendo esto a la vez.
    // Para esto uso el semaforo de espera. ingresan todas una persona cuando queda un espacio, y superan la capacidad.
    mutex->wait();
    // Esperar a que haya lugar en la sala
    if (sala->cantidad == ROOM_SIZE)
    {
        ss << "\033[1;39m" << owner << " llena\033[0m" << std::endl;
        Helper::output(stdout, ss);
        sala->estadoPuerta[numero] = WAITING;
        mutex->post();
        llena->wait();
        mutex->wait();
        sala->estadoPuerta[numero] = WORKING;
    }
    mutex->post();
    // Avisar a la persona que puede entrar
    long idPersona = msg.sender;
    msg.type = msg.sender;
    msg.sender = myId;
    msg.message = ENTRA;
    ss << owner << " enviando " << Helper::msgToString(msg.message) << " a " << (msg.type - M_PERS) << " (" << (msg.type) << ")" << std::endl;
    Helper::output(stdout, ss);
    persona->send(&msg);
    mutex->wait();
    // Guardar los datos de la persona
    sala->personas[sala->pWrite] = idPersona;
    sala->pWrite = (sala->pWrite + 1 ) % ROOM_SIZE;
    sala->cantidad++;
    // Avisar al cablecarril si estaba esperando
    if (sala->cantidad == 1)
    {
        for (int i = 0; i < CC_AMOUNT; i++)
        {
            if (registro->cc[i].estado == WAITING)
            {
                ss << owner << " estaba vacia despertando al cablecarril " << i << std::endl;
                Helper::output(stdout, ss);
                registro->cc[i].estado = WORKING;
                vacia->post(i);
            }
        }
    }
    mutex->post();
    espera->post();
}
