#include "Sala.h"

Sala::Sala() {
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

void Sala::init() {
    std::stringstream ss;
    struct iMessage msg;
    pid_t pid;
    while (true) {
        in->receive(&msg, 0);
        pid = fork();
        if (pid == 0) {
            ss << owner << " recibi " << Helper::msgToString(msg.message) << std::endl;
            Helper::output(stdout, ss);
            switch (msg.message) {
                case ESPERAR_PERSONA_ABAJO:
                    msg.type = msg.sender;
                    msg.sender = M_SALA_ABAJO;
                    msg.message = ENTRO_PERSONA_ABAJO;
                    esperarPersona(BOTTOM);
                    break;
                case ESPERAR_PERSONA_ARRIBA:
                    msg.type = msg.sender;
                    msg.sender = M_SALA_ARRIBA;
                    msg.message = ENTRO_PERSONA_ARRIBA;
                    esperarPersona(TOP);
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
        } else if (pid < 0) {
            perror("Sala: fork");
        }
    }
}

void Sala::esperarPersona(enum location ubicacion) {
    std::stringstream ss;
    struct personMessage msg;
    Semaphore * llena;
    Queue<struct personMessage> * myQueue;
    long myId;
    struct sala * sala;
    std::string owner = this->owner + " de " + Helper::msgToString(ubicacion);
    if (ubicacion == BOTTOM) {
        sala = &registro->abajo;
        myQueue = salaAbajo;
        myId = M_SALA_ABAJO;
        llena = new Semaphore(this->llena, 0);
    } else {
        sala = &registro->arriba;
        myQueue = salaArriba;
        myId = M_SALA_ARRIBA;
        llena = new Semaphore(this->llena, 1);
    }
    // Esperar a que alguien quiera entrar
    ss << owner << " esperando persona " << std::endl;
    Helper::output(stdout, ss);
    myQueue->receive(&msg, myId);
    ss << owner << " recibi: " << Helper::msgToString(msg.message) << " de " << (msg.sender) << std::endl;
    Helper::output(stdout, ss);
    if (msg.message != QUIERO_ENTRAR) {
        ss << owner << " \033[41m\033[30mError\33[0m mensaje incorrecto" << Helper::msgToString(msg.message);
        Helper::output(stderr, ss);
        exit(EXIT_FAILURE);
    }
    // Esperar a que haya lugar en la sala
    mutex->wait();
    if (sala->cantidad == ROOM_SIZE) {
        ss << owner << " llena" << std::endl;
        Helper::output(stdout, ss);
        sala->estado = WAITING;
        mutex->post();
        llena->wait();
        mutex->wait();
        sala->estado = WORKING;
    }
    mutex->post();
    // Avisar a la persona que puede entrar
    long idPersona = msg.sender;
    msg.type = msg.sender;
    msg.sender = myId;
    msg.message = ENTRA;
    ss << owner << " enviando " << Helper::msgToString(msg.message) << " a " << (msg.type) << std::endl;
    Helper::output(stdout, ss);
    persona->send(&msg);
    mutex->wait();
    // Guardar los datos de la persona
    sala->personas[sala->pWrite] = idPersona;
    sala->pWrite++;
    if (sala->pWrite == ROOM_SIZE) {
        sala->pWrite = 0;
    }
    sala->cantidad++;
    // Avisar al cablecarril si estaba esperando
    if (sala->cantidad == 1 && registro->cc.estado == WAITING) {
        ss << owner << " estaba vacia despertando al cablecarril" << std::endl;
        Helper::output(stdout, ss);
        registro->cc.estado = WORKING;
        vacia->post();
    }
    mutex->post();
}
