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
    shm = new SharedMemory<struct registro>(PATH, SHM_CC_SALAS, owner);
    shm->get();
    registro = shm->attach();
    mutex = new Semaphore(PATH, SEM_MUTEX, owner);
    mutex->get();
    llenaAbajo = new SemaphoreArray(PATH, SEM_FULL_BOTTOM, DOOR_AMOUNT, owner);
    llenaAbajo->get();
    llenaArriba = new SemaphoreArray(PATH, SEM_FULL_TOP, DOOR_AMOUNT, owner);
    llenaArriba->get();
    salasVacias = new SemaphoreArray(PATH, SEM_EMPTY, CC_AMOUNT, owner);
    salasVacias->get();
}

void CableCarril::init()
{
    std::stringstream ss;
    struct iMessage msg;
    pid_t pid;
    while (true)
    {
        in->receive(&msg, 0);
        ss << "Componente " << owner << " recibi " << Helper::msgToString(msg.message) << " de " << msg.sender - M_CABLE_CARRIL << " (" << msg.sender << ")" << std::endl;
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
                    llegar(ABAJO, msg.number, msg.data.people);
                    msg.message = BAJARON_PASAJEROS_ABAJO;
                    break;
                case ESTOY_ARRIBA:
                    llegar(ARRIBA, msg.number, msg.data.people);
                    msg.message = BAJARON_PASAJEROS_ARRIBA;
                    break;
                case CARGAR_PERSONAS_ABAJO:
                    cargarPersonas(ABAJO, msg.number, msg.data.people);
                    msg.message = TERMINE_DE_CARGAR_PERSONAS_ABAJO;
                    break;
                case CARGAR_PERSONAS_ARRIBA:
                    cargarPersonas(ARRIBA, msg.number, msg.data.people);
                    msg.message = TERMINE_DE_CARGAR_PERSONAS_ARRIBA;
                    break;
                default:
                    ss << owner << "\033[41m\033[30mError\33[0m mensaje incorrecto" << Helper::msgToString(msg.message) << std::endl;
                    Helper::output(stderr, ss);
                    exit(EXIT_FAILURE);
            }
            msg.type = msg.sender;
            msg.sender = M_CABLE_CARRIL;
            out->send(&msg);
            ss << "Componente " << owner << " envie " << Helper::msgToString(msg.message) << " a " << msg.type - M_CABLE_CARRIL << " (" << msg.type << ")" << std::endl;
            Helper::output(stdout, ss);
            exit(EXIT_SUCCESS);
        }
    }
}

void CableCarril::llegar(enum location location, long numero, long personas[CC_SIZE])
{
    std::stringstream ss;
    unsigned int pasajeros;
    struct personMessage msg;
    mutex->wait();
    registro->cc[numero].estado = WORKING;
    registro->cc[numero].ubicacion = location;
    registro->cc[numero].cantidad = 0;
    mutex->post();
    long myId = M_CABLE_CARRIL + numero;
    ss << this->owner << " " << numero << " (" << myId << ")";
    std::string owner = ss.str();
    ss.str("");
    for (pasajeros = 0; pasajeros < CC_SIZE && personas[pasajeros] != 0; pasajeros++);
    ss << owner << " \033[36mtengo " << pasajeros << " para bajar\033[0m" << std::endl;
    Helper::output(stdout, ss);
    while (pasajeros)
    {
        pasajeros--;
        // Indicar al pasajero que baje
        msg.type = personas[pasajeros];
        msg.sender = myId;
        msg.message = BAJAR;
        persona->send(&msg);
        ss << owner << " envie " << Helper::msgToString(msg.message) << " a " << msg.type << std::endl;
        Helper::output(stdout, ss);
        // Esperar a que el pasajero indique que bajo
        cc->receive(&msg, myId);
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

void CableCarril::cargarPersonas(enum location ubicacion, long numero, long personas[CC_SIZE])
{
    std::stringstream ss;
    struct personMessage msg;
    SemaphoreArray * llena;
    Semaphore * vacia = new Semaphore(this->salasVacias, numero);
    struct sala * sala;
    long myId = M_CABLE_CARRIL + numero;
    ss << this->owner << " " << numero << " (" << myId << ")";
    std::string owner = ss.str();
    ss.str("");
    if (ubicacion == ABAJO)
    {
        llena = this->llenaAbajo;
        sala = &(registro->abajo);
    } else
    {
        llena = this->llenaArriba;
        sala = &(registro->arriba);
    }
    mutex->wait();
    // Esperar si no hay nadie en ambas salas.
    // TODO poner en cola los cablecarriles en ambas salas, es decir, que solo trabaje 1 cablecarril a la vez en cada sala.
    if (registro->abajo.cantidad == 0 && registro->arriba.cantidad == 0)
    {
        registro->cc[numero].estado = WAITING;
        mutex->post();
        ss << "\033[1;36m" << owner << " ambas salas vacias, esperando " << Helper::msgToString(ubicacion) << "\033[0m" << std::endl;
        Helper::output(stdout, ss);
        vacia->wait();
        ss << owner << " dejo de esperar " << Helper::msgToString(ubicacion) << std::endl;
        Helper::output(stdout, ss);
        mutex->wait();
        registro->cc[numero].estado = WORKING;
    }
    unsigned int pasajeros = 0;
    // Cargar las personas que esten en la sala
    while (pasajeros < CC_SIZE && sala->cantidad > 0)
    {
        // Sacar una persona de la sala
        personas[pasajeros] = sala->personas[sala->pRead];
        sala->pRead = (sala->pRead + 1) % ROOM_SIZE;
        sala->cantidad--;
        // Si la sala estaba llena, avisar a la sala que puede seguir ingresando personas
        // TODO solo despertamos a la primera disponible, podria hacerse un round robin en este lugar
        bool wokeUp = false;
        if (sala->cantidad == ROOM_SIZE - 1)
        {
            for (int i = 0; i < DOOR_AMOUNT && !wokeUp; i++)
            {
                if (sala->estadoPuerta[i] == WAITING)
                {
                    ss << owner << "\033[1;39m " << Helper::msgToString(ubicacion) << " estaba lleno, despertando a puerta " << i << " de la sala de " << Helper::msgToString(ubicacion) << "\033[0m" << std::endl;
                    Helper::output(stdout, ss);
                    llena->post(i);
                    sala->estadoPuerta[i] = WORKING;
                    wokeUp = true;
                }
            }
        }
        mutex->post();
        // Avisar a la persona que suba
        msg.type = personas[pasajeros];
        msg.sender = myId;
        msg.message = SUBIR;
        ss << owner << " enviando " << Helper::msgToString(msg.message) << " a " << msg.type << std::endl;
        Helper::output(stdout, ss);
        persona->send(&msg);
        // Esperar que la persona conteste que subio
        cc->receive(&msg, myId);
        pasajeros++;
        if (msg.message == SUBI)
        {
            ss << owner << " recibi " << Helper::msgToString(msg.message) << " de " << msg.sender << std::endl;
            ss << owner << "\033[36m tengo " << pasajeros << "\033[0m" << std::endl;
            Helper::output(stdout, ss);
        } else
        {
            ss << owner << " \033[41m\033[30mError\33[0m mensaje incorrecto: " << Helper::msgToString(msg.message) << " esperaba: " << Helper::msgToString(SUBI) << std::endl;
            Helper::output(stderr, ss);
        }
        mutex->wait();
        registro->cc[numero].cantidad++;
        ss << owner << " \033[36mQuedan " << registro->abajo.cantidad << " abajo y " << registro->arriba.cantidad << " arriba\033[0m" << std::endl;
        Helper::output(stdout, ss);
    }
    registro->cc[numero].estado = TRAVELLING;
    if (sala->cantidad > 0)
    {
        for (int i = 0; i < CC_AMOUNT; i++)
        {
            if (registro->cc[i].estado == WAITING)
            {
                ss << owner << "\033[1;39m me voy y queda gente despertando al cablecarril " << i << "\033[0m" << std::endl;
                Helper::output(stdout, ss);
                registro->cc[i].estado = WORKING;
                this->salasVacias->post(i);
            }
        }
    }
    mutex->post();
    personas[pasajeros] = 0;
    ss << owner << " Cargue " << pasajeros << " persona" << (pasajeros == 1 ? "" : "s") << std::endl;
    Helper::output(stdout, ss);
}
