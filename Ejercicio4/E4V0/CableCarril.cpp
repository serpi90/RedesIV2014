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

void CableCarril::init()
{
    std::stringstream ss;
    struct iMessage msg;
    pid_t pid;
    while (true)
    {
        in->receive(&msg, 0);
        ss << owner << " recibi " << Helper::msgToString(msg.message) << " (" << msg.sender << ")" << std::endl;
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
                    cargarPersonas(BOTTOM, msg.data.people);
                    msg.message = CARGAR_PERSONAS_ABAJO_OK;
                    break;
                case CARGAR_PERSONAS_ARRIBA:
                    cargarPersonas(TOP, msg.data.people);
                    msg.message = CARGAR_PERSONAS_ARRIBA_OK;
                    break;
                default:
                    ss << owner << "\033[41m\033[30mError\33[0m mensaje incorrecto" << Helper::msgToString(msg.message) << std::endl;
                    Helper::output(stderr, ss);
                    exit(EXIT_FAILURE);
            }
            msg.type = msg.sender;
            msg.sender = M_CABLE_CARRIL;
            out->send(&msg);
            ss << owner << " envie " << Helper::msgToString(msg.message) << " (" << msg.type << ")" << std::endl;
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
    registro->cc.estado = WORKING;
    registro->cc.ubicacion = location;
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
        if (msg.message == BAJE)
        {
            ss << owner << " recibi " << Helper::msgToString(msg.message) << " de " << msg.sender << std::endl;
            ss << owner << " quedan " << pasajeros << " pasajeros por bajar " << std::endl;
            Helper::output(stdout, ss);
        } else
        {
            ss << owner << "\033[41m\033[30mError\33[0m mensaje incorrecto: " << Helper::msgToString(msg.message) << " esperaba: " << Helper::msgToString(BAJE) << std::endl;
            Helper::output(stderr, ss);
        }
    }
}

void CableCarril::cargarPersonas(enum location ubicacion, long personas[CC_SIZE])
{
    std::stringstream ss;
    Semaphore * llenaArriba = new Semaphore(llena, 1);
    struct personMessage msg;
    struct sala * sala = (ubicacion == BOTTOM) ? &registro->abajo : &registro->arriba;
    std::string nombreSala = (ubicacion == BOTTOM) ? "abajo" : "arriba";
    mutex->wait();
    // Esperar si no hay nadie en ambas salas.
    if (registro->abajo.cantidad == 0 && registro->arriba.cantidad == 0)
    {
        registro->cc.estado = WAITING;
        mutex->post();
        ss << owner << " sala de " << nombreSala << " vacia, esperando" << std::endl;
        Helper::output(stdout, ss);
        vacia->wait();
        ss << owner << " dejo de esperar " << nombreSala << std::endl;
        Helper::output(stdout, ss);
        mutex->wait();
        registro->cc.estado = WORKING;
    }
    int cantPersonas = 0;
    // Cargar las personas que esten en la sala
    while (cantPersonas < CC_SIZE && sala->cantidad > 0)
    {
        personas[cantPersonas] = sala->personas[sala->pRead];
        sala->pRead++;
        sala->cantidad--;
        // Si la sala estaba llena, avisar a la sala que puede seguir ingresando personas
        if (sala->cantidad == ROOM_SIZE - 1 && sala->estado == WAITING)
        {
            ss << nombreSala << " estaba lleno, despertando a la sala de " << nombreSala << std::endl;
            Helper::output(stdout, ss);
            llenaArriba->post();
            sala->estado = WORKING;
        }
        mutex->post();
        // Avisar a la persona que suba
        msg.type = personas[cantPersonas];
        msg.sender = M_CABLE_CARRIL;
        msg.message = SUBIR;
        ss << owner << " enviando " << Helper::msgToString(msg.message) << " a " << msg.type << std::endl;
        Helper::output(stdout, ss);
        persona->send(&msg);
        // Esperar que la persona conteste que subio
        cc->receive(&msg, M_CABLE_CARRIL);
        cantPersonas++;
        if (msg.message == SUBI)
        {
            ss << owner << " recibi " << Helper::msgToString(msg.message) << " de " << msg.sender << std::endl;
            ss << owner << " subieron " << cantPersonas << std::endl;
            Helper::output(stdout, ss);
        } else
        {
            ss << owner << "\033[41m\033[30mError\33[0m mensaje incorrecto: " << Helper::msgToString(msg.message) << " esperaba: " << Helper::msgToString(BAJE) << std::endl;
            Helper::output(stderr, ss);
        }
        mutex->wait();
        ss << owner << " Hay " << registro->abajo.cantidad << " abajo y " << registro->arriba.cantidad << " arriba" << std::endl;
        Helper::output(stdout, ss);
    }
    registro->cc.estado = TRAVELLING;
    mutex->post();
    personas[cantPersonas] = 0;
    ss << owner << " Cargue " << cantPersonas << " persona" << (cantPersonas == 1 ? "" : "s") << std::endl;
    Helper::output(stdout, ss);
}
