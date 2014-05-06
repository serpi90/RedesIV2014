#include "CableCarril.h"

CableCarril::CableCarril()
{
    owner = "CableCarril";
    in = new Queue<struct iMessage>(PATH, Q_CC_FROM_INTERFACE, owner);
    in->get();
    out = new Queue<struct iMessage>(PATH, Q_CC_TO_INTERFACE, owner);
    out->get();
    persona = new Queue<struct personMessage>(PATH, Q_PERSONA, owner);
    persona->get();
    cc = new Queue<struct personMessage>(PATH, Q_CC, owner);
    cc->get();
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

void CableCarril::init()
{
    std::stringstream ss;
    struct iMessage msg;
    pid_t pid;
    while (true)
    {
        in->receive(&msg, 0);
        ss << owner << " recibi " << Helper::msgToString(msg.message) << std::endl;
        Helper::output(stdout, ss);
        pid = fork();
        if (pid < 0)
        {
            perror("CableCarril: fork");
        } else if (pid == 0)
        {
            switch (msg.message)
            {
                case ESTOY_ABAJO:
                    estoy(BOTTOM, msg.data.people);
                    msg.message = ESTOY_ABAJO_OK;
                    break;
                case ESTOY_ARRIBA:
                    estoy(TOP, msg.data.people);
                    msg.message = ESTOY_ARRIBA_OK;
                    break;
                case CARGAR_PERSONAS_ABAJO:
                    cargarPersonasAbajo(msg.data.people);
                    msg.message = CARGAR_PERSONAS_ABAJO_OK;
                    break;
                case CARGAR_PERSONAS_ARRIBA:
                    cargarPersonasArriba(msg.data.people);
                    msg.message = CARGAR_PERSONAS_ARRIBA_OK;
                    break;
                default:
                    ss << owner << "mensaje incorrecto" << Helper::msgToString(msg.message) << std::endl;
                    Helper::output(stderr, ss);
                    exit(EXIT_FAILURE);
            }
            msg.type = msg.sender;
            msg.sender = M_CABLE_CARRIL;
            out->send(&msg);
            ss << owner << " envie " << Helper::msgToString(msg.message) << std::endl;
            Helper::output(stdout, ss);
            exit(EXIT_SUCCESS);
        }
    }
}

void CableCarril::estoy(enum location location, long personas[CC_SIZE])
{
    std::stringstream ss;
    int pasajeros;
    struct personMessage msg;
    mutex->wait();
    cableCarril->estadoCablecarril = WORKING;
    cableCarril->ubicacionCablecarril = location;
    mutex->post();
    for (pasajeros = 0; pasajeros < CC_SIZE && personas[pasajeros] != 0; pasajeros++);
    ss << owner << " tengo " << pasajeros << " para bajar" << std::endl;
    Helper::output(stdout, ss);
    while (pasajeros)
    {
        pasajeros--;
        msg.type = personas[pasajeros];
        msg.sender = M_CABLE_CARRIL;
        msg.message = BAJAR;
        persona->send(&msg);
        ss << owner << " envie " << Helper::msgToString(msg.message) << " a " << msg.type << std::endl;
        Helper::output(stdout, ss);
        cc->receive(&msg, M_CABLE_CARRIL);
        ss << owner << " recibi " << Helper::msgToString(msg.message) << " de " << msg.sender << std::endl;
        ss << owner << " quedan " << pasajeros << " pasajeros por bajar " << std::endl;
        Helper::output(stdout, ss);
        // TODO verificar msg.message == BAJE
    }
}

void CableCarril::cargarPersonasAbajo(long personas[CC_SIZE])
{
    std::stringstream ss;
    Semaphore * llenaAbajo = new Semaphore(llena, 0);
    struct personMessage msg;
    mutex->wait();
    if (cableCarril->cantAbajo == 0 && cableCarril->cantArriba == 0)
    {
        cableCarril->estadoCablecarril = WAITING;
        mutex->post();
        ss << owner << " sala de abajo vacia, esperando" << std::endl;
        Helper::output(stdout, ss);
        vacia->wait();
        ss << owner << " sala de abajo ya no esta vacia" << std::endl;
        Helper::output(stdout, ss);
        mutex->wait();
        cableCarril->estadoCablecarril = WORKING;
    }
    int pasajeros = 0;
    // Mientras haya personas
    while (pasajeros < CC_SIZE && cableCarril->cantAbajo != 0)
    {
        personas[pasajeros] = cableCarril->personasAbajo[cableCarril->pReadAbajo];
        cableCarril->pReadAbajo++;
        cableCarril->cantAbajo--;
        if (cableCarril->cantAbajo == ROOM_SIZE - 1 && cableCarril->estadoAbajo == WAITING)
        {
            Helper::output(stdout, "Abajo estaba lleno despertando a la sala de abajo\n");
            llenaAbajo->post();
            cableCarril->estadoAbajo = WORKING;
        }

        mutex->post();
        msg.type = personas[pasajeros];
        msg.sender = M_CABLE_CARRIL;
        msg.message = SUBIR;
        ss << owner << " enviando " << Helper::msgToString(msg.message) << " a " << msg.type << std::endl;
        Helper::output(stdout, ss);
        persona->send(&msg);
        cc->receive(&msg, M_CABLE_CARRIL);
        ss << owner << " recibi " << Helper::msgToString(msg.message) << " de " << msg.sender << std::endl;
        Helper::output(stdout, ss);
        //TODO verificar msg.message = SUBI;
        pasajeros++;
        mutex->wait();
    }
    mutex->post();
    personas[pasajeros] = 0;
    ss << owner << " Cargue " << pasajeros << " persona" << (pasajeros == 1 ? "" : "s") << std::endl;
    Helper::output(stdout, ss);
}

void CableCarril::cargarPersonasArriba(long personas[CC_SIZE])
{
    std::stringstream ss;
    Semaphore * llenaArriba = new Semaphore(llena, 1);
    struct personMessage msg;
    mutex->wait();
    if (cableCarril->cantAbajo == 0 && cableCarril->cantArriba == 0)
    {
        cableCarril->estadoCablecarril = WAITING;
        mutex->post();
        ss << owner << " sala de arriba vacia, esperando" << std::endl;
        Helper::output(stdout, ss);
        vacia->wait();
        ss << owner << " sala de arriba ya no esta vacia" << std::endl;
        Helper::output(stdout, ss);
        mutex->wait();
        cableCarril->estadoCablecarril = WORKING;
    }
    int pasajeros = 0;
    // Mientras haya personas
    while (pasajeros < CC_SIZE && cableCarril->cantArriba != 0)
    {
        personas[pasajeros] = cableCarril->personasArriba[cableCarril->pReadArriba];
        cableCarril->pReadArriba++;
        cableCarril->cantArriba--;
        if (cableCarril->cantArriba == ROOM_SIZE - 1 && cableCarril->estadoArriba == WAITING)
        {
            Helper::output(stdout, "Arriba estaba lleno despertando a la sala de arriba\n");
            llenaArriba->post();
            cableCarril->estadoArriba = WORKING;
        }

        mutex->post();
        msg.type = personas[pasajeros];
        msg.sender = M_CABLE_CARRIL;
        msg.message = SUBIR;
        ss << owner << " enviando " << Helper::msgToString(msg.message) << " a " << msg.type << std::endl;
        Helper::output(stdout, ss);
        persona->send(&msg);
        cc->receive(&msg, M_CABLE_CARRIL);
        ss << owner << " recibi " << Helper::msgToString(msg.message) << " de " << msg.sender << std::endl;
        Helper::output(stdout, ss);
        //TODO verificar msg.message = SUBI;
        pasajeros++;
        mutex->wait();
    }
    mutex->post();
    personas[pasajeros] = 0;
    ss << owner << " Cargue " << pasajeros << " persona" << (pasajeros == 1 ? "" : "s") << std::endl;
    Helper::output(stdout, ss);
}
