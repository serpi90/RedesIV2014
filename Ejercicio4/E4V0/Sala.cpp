#include "Sala.h"

Sala::Sala()
{
    owner = "Sala";
    in = new Queue<struct iMessage>(PATH, Q_SALA_FROM_INTERFACE, owner);
    in->get();
    out = new Queue<struct iMessage>(PATH, Q_SALA_TO_INTERFACE, owner);
    out->get();
    persona = new Queue<struct personMessage>(PATH, Q_PERSONA, owner);
    persona->get();
    salaAbajo = new Queue<struct personMessage>(PATH, Q_SALA_ABAJO, owner);
    salaAbajo->get();
    salaArriba = new Queue<struct personMessage>(PATH, Q_SALA_ARRIBA, owner);
    salaArriba->get();
    shm = new SharedMemory<struct registro>(PATH, SHM_CC, owner);
    shm->get();
    registro = shm->attach();
    mutex = new Semaphore(PATH, SEM_MUTEX, owner);
    mutex->get();
    llena = new SemaphoreArray(PATH, SEM_FULL, 2, owner);
    llena->get();
    vacia = new Semaphore(PATH, SEM_EMPTY, owner);
    vacia->get();
}

void Sala::init()
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
            ss << owner << " recibi " << Helper::msgToString(msg.message) << std::endl;
            Helper::output(stdout, ss);
            switch (msg.message)
            {
                case ESPERAR_PERSONA_ABAJO:
                    msg.type = msg.sender;
                    msg.sender = M_SALA_ABAJO;
                    msg.message = ESPERAR_PERSONA_ABAJO_OK;
                    ingresarPersona(BOTTOM);
                    break;
                case ESPERAR_PERSONA_ARRIBA:
                    msg.type = msg.sender;
                    msg.sender = M_SALA_ARRIBA;
                    msg.message = ESPERAR_PERSONA_ARRIBA_OK;
                    ingresarPersona(TOP);
                    break;
                default:
                    ss << owner << " \033[41m\033[30mError\33[0m mensaje incorrecto" << Helper::msgToString(msg.message);
                    Helper::output(stderr, ss);
                    exit(EXIT_FAILURE);
            }
            out->send(&msg);
            ss << owner << " envie " << Helper::msgToString(msg.message) << std::endl;
            Helper::output(stdout, ss);
            exit(EXIT_SUCCESS);
        } else if (pid < 0)
        {
            perror("Sala: fork");
        }
    }
}

void Sala::ingresarPersona(enum location location)
{
    std::stringstream ss;
    struct personMessage msg;
    Semaphore * semLlena;
    Queue<struct personMessage> * q;
    std::string nombreSala;
    long myId;
    struct sala * sala;
    if (location == BOTTOM)
    {
        nombreSala = "abajo";
        sala = &registro->abajo;
        q = salaAbajo;
        myId = M_SALA_ABAJO;
        semLlena = new Semaphore(llena, 0);
    } else
    {
        nombreSala = "arriba";
        sala = &registro->arriba;
        q = salaArriba;
        myId = M_SALA_ARRIBA;
        semLlena = new Semaphore(llena, 1);
    }
    ss << owner << " esperando persona" << std::endl;
    Helper::output(stdout, ss);
    q->receive(&msg, myId);
    ss << owner << " recibi: " << Helper::msgToString(msg.message) << " de " << (msg.sender) << std::endl;
    Helper::output(stdout, ss);
    if (msg.message != QUIERO_ENTRAR)
    {
        ss << owner << " \033[41m\033[30mError\33[0m mensaje incorrecto" << Helper::msgToString(msg.message);
        Helper::output(stderr, ss);
        exit(EXIT_FAILURE);
    }
    mutex->wait();
    if (sala->cantidad == ROOM_SIZE)
    {
        ss << owner << " sala de " << nombreSala << " llena" << std::endl;
        Helper::output(stdout, ss);
        sala->estado = WAITING;
        mutex->post();
        semLlena->wait();
        mutex->wait();
    }

    sala->personas[sala->pWrite] = msg.sender;
    sala->pWrite++;
    sala->cantidad++;

    bool wasFull = sala->cantidad == 1;
    bool ccWaiting = registro->cc.estado == WAITING;
    mutex->post();

    msg.type = msg.sender;
    msg.sender = myId;
    msg.message = ENTRA;
    ss << owner << " enviando " << Helper::msgToString(msg.message) << " a " << (msg.type) << std::endl;
    Helper::output(stdout, ss);
    persona->send(&msg);
    if (wasFull && ccWaiting)
    {
        ss << owner << " de " << nombreSala << " estaba vacia despertando al cablecarril" << std::endl;
        Helper::output(stdout, ss);
        vacia->post();
    }
}
