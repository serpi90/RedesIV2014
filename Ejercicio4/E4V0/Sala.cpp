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
    std::string s;
    struct iMessage msg;
    pid_t pid;
    while (true)
    {
        in->receive(&msg, 0);
        ss << owner << " recibi " << Helper::msgToString(msg.message) << std::endl;
        if (msg.message != ESPERAR_PERSONA_ABAJO && msg.message != ESPERAR_PERSONA_ARRIBA)
        {
            ss << owner << " mensaje incorrecto" << Helper::msgToString(msg.message);
            Helper::output(stderr, ss);
        } else
        {
            pid = fork();
            if (pid < 0)
            {
                perror("Sala: fork");
            } else if (pid == 0)
            {
                bool abajo = msg.message == ESPERAR_PERSONA_ABAJO;
                Helper::output(stdout, s);
                ingresarPersona(abajo);
                msg.type = msg.sender;
                msg.sender = abajo ? M_SALA_ABAJO : M_SALA_ARRIBA;
                msg.message = abajo ? ESPERAR_PERSONA_ABAJO_OK : ESPERAR_PERSONA_ARRIBA_OK;
                out->send(&msg);
                ss << owner << " envie " << Helper::msgToString(msg.message) << std::endl;
                Helper::output(stdout, ss);
                exit(EXIT_SUCCESS);
            }
        }
    }
}

void Sala::ingresarPersona(bool abajo)
{
    std::stringstream ss;
    struct personMessage msg;
    Queue<struct personMessage> * q = abajo ? salaAbajo : salaArriba;
    Semaphore * semLlena = new Semaphore(llena, abajo ? 0 : 1);
    ss << owner << " esperando persona" << std::endl;
    Helper::output(stdout, ss);
    q->receive(&msg, abajo ? M_SALA_ABAJO : M_SALA_ARRIBA);
    ss << owner << " recibi: " << Helper::msgToString(msg.message) << " de " << (msg.sender) << std::endl;
    Helper::output(stdout, ss);
    if (msg.message != QUIERO_ENTRAR)
    {
        ss << owner << "mensaje incorrecto" << Helper::msgToString(msg.message);
        Helper::output(stderr, ss);
        exit(EXIT_FAILURE);
    }
    mutex->wait();
    bool full = (abajo ? cableCarril->cantAbajo : cableCarril->cantArriba) == ROOM_SIZE;
    if (full)
    {
        ss << owner << " sala de " << (abajo ? "abajo" : "arriba") << " llena" << std::endl;
        Helper::output(stdout, ss);
        if (abajo)
        {
            cableCarril->estadoAbajo = WAITING;
        } else
        {
            cableCarril->estadoArriba = WAITING;
        }
        mutex->post();
        semLlena->wait();
        mutex->wait();
    }
    if (abajo)
    {
        cableCarril->personasAbajo[cableCarril->pWriteAbajo] = msg.sender;
        cableCarril->pWriteAbajo++;
        cableCarril->cantAbajo++;
    } else
    {
        cableCarril->personasArriba[cableCarril->pWriteArriba] = msg.sender;
        cableCarril->pWriteArriba++;
        cableCarril->cantArriba++;
    }
    bool wasFull = (abajo ? cableCarril->cantAbajo : cableCarril->cantArriba) == 1;
    bool cablecarrilEsperando = cableCarril->estadoCablecarril == WAITING;
    mutex->post();

    msg.type = msg.sender;
    msg.sender = abajo ? M_SALA_ABAJO : M_SALA_ARRIBA;
    msg.message = ENTRA;
    ss << owner << " enviando " << Helper::msgToString(msg.message) << " a " << (msg.type) << std::endl;
    Helper::output(stdout, ss);
    persona->send(&msg);
    if (wasFull && cablecarrilEsperando)
    {
        ss << owner << " de " << (abajo ? "abajo" : "arriba") << " estaba vacia despertando al cablecarril" << std::endl;
        Helper::output(stdout, ss);
        vacia->post();
    }
}