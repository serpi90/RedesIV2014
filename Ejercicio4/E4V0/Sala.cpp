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
    shm = new SharedMemory<struct cableCarril>(PATH, SHM_CC, owner);
    shm->get();
    cableCarril = shm->attach();
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
                    ingresarPersonaAbajo();
                    break;
                case ESPERAR_PERSONA_ARRIBA:
                    msg.type = msg.sender;
                    msg.sender = M_SALA_ARRIBA;
                    msg.message = ESPERAR_PERSONA_ARRIBA_OK;
                    ingresarPersonaArriba();
                    break;
                default:
                    ss << owner << " mensaje incorrecto" << Helper::msgToString(msg.message);
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

void Sala::ingresarPersonaAbajo()
{
    std::stringstream ss;
    struct personMessage msg;
    Queue<struct personMessage> * q = salaAbajo;
    Semaphore * semLlena = new Semaphore(llena, 0);
    ss << owner << " esperando persona" << std::endl;
    Helper::output(stdout, ss);
    q->receive(&msg, M_SALA_ABAJO);
    ss << owner << " recibi: " << Helper::msgToString(msg.message) << " de " << (msg.sender) << std::endl;
    Helper::output(stdout, ss);
    if (msg.message != QUIERO_ENTRAR)
    {
        ss << owner << "mensaje incorrecto" << Helper::msgToString(msg.message);
        Helper::output(stderr, ss);
        exit(EXIT_FAILURE);
    }
    mutex->wait();
    if (cableCarril->cantAbajo == ROOM_SIZE)
    {
        ss << owner << " sala de abajo llena" << std::endl;
        Helper::output(stdout, ss);
        cableCarril->estadoAbajo = WAITING;
        mutex->post();
        semLlena->wait();
        mutex->wait();
    }

    cableCarril->personasAbajo[cableCarril->pWriteAbajo] = msg.sender;
    cableCarril->pWriteAbajo++;
    cableCarril->cantAbajo++;

    bool wasFull = cableCarril->cantAbajo == 1;
    bool ccWaiting = cableCarril->estadoCablecarril == WAITING;
    mutex->post();

    msg.type = msg.sender;
    msg.sender = M_SALA_ABAJO;
    msg.message = ENTRA;
    ss << owner << " enviando " << Helper::msgToString(msg.message) << " a " << (msg.type) << std::endl;
    Helper::output(stdout, ss);
    persona->send(&msg);
    if (wasFull && ccWaiting)
    {
        ss << owner << " de abajo estaba vacia despertando al cablecarril" << std::endl;
        Helper::output(stdout, ss);
        vacia->post();
    }
}

void Sala::ingresarPersonaArriba()
{
    std::stringstream ss;
    struct personMessage msg;
    Queue<struct personMessage> * q = salaArriba;
    Semaphore * semLlena = new Semaphore(llena, 0);
    ss << owner << " esperando persona" << std::endl;
    Helper::output(stdout, ss);
    q->receive(&msg, M_SALA_ARRIBA);
    ss << owner << " recibi: " << Helper::msgToString(msg.message) << " de " << (msg.sender) << std::endl;
    Helper::output(stdout, ss);
    if (msg.message != QUIERO_ENTRAR)
    {
        ss << owner << "mensaje incorrecto" << Helper::msgToString(msg.message);
        Helper::output(stderr, ss);
        exit(EXIT_FAILURE);
    }
    mutex->wait();
    if (cableCarril->cantArriba == ROOM_SIZE)
    {
        ss << owner << " sala de arriba llena" << std::endl;
        Helper::output(stdout, ss);
        cableCarril->estadoArriba = WAITING;
        mutex->post();
        semLlena->wait();
        mutex->wait();
    }

    cableCarril->personasArriba[cableCarril->pWriteArriba] = msg.sender;
    cableCarril->pWriteArriba++;
    cableCarril->cantArriba++;

    bool wasFull = cableCarril->cantArriba == 1;
    bool ccWaiting = cableCarril->estadoCablecarril == WAITING;
    mutex->post();

    msg.type = msg.sender;
    msg.sender = M_SALA_ARRIBA;
    msg.message = ENTRA;
    ss << owner << " enviando " << Helper::msgToString(msg.message) << " a " << (msg.type) << std::endl;
    Helper::output(stdout, ss);
    persona->send(&msg);
    if (wasFull && ccWaiting)
    {
        ss << owner << " de arriba estaba vacia despertando al cablecarril" << std::endl;
        Helper::output(stdout, ss);
        vacia->post();
    }
}