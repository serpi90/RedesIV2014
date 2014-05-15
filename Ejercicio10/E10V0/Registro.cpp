#include <sstream>
#include <string>
#include "Registro.h"
#include "Helper.h"
#include "Config.h"

Registro::Registro()
{
    this->owner = "Registro";
    in = new Queue<struct iMessage>(PATH, Q_FROM_INTERFACE, owner);
    in->get();
    out = new Queue<struct iMessage>(PATH, Q_TO_INTERFACE, owner);
    out->get();
    mutex = new Semaphore(PATH, SEM_MUTEX, owner);
    mutex->get();
    esperaLectores = new Semaphore(PATH, SEM_LECTORES, owner);
    esperaLectores->get();
    esperaEscritores = new Semaphore(PATH, SEM_ESCRITORES, owner);
    esperaEscritores->get();
    cotiz = new Semaphore(PATH, SEM_COTIZ, owner);
    cotiz->get();
    shm = new SharedMemory<struct registro>(PATH, SHM_REGISTRO, owner);
    shm->get();
    this->registro = shm->attach();
    Config conf("config.conf");
    lecMin = conf.getInt("lectura min", 1);
    lecMax = conf.getInt("lectura mxn", 5);
    escMin = conf.getInt("escritura min", 1);
    escMax = conf.getInt("escritura max", 5);
}

void Registro::init()
{
    std::stringstream ss;
    struct iMessage msg;
    pid_t pid;
    while (true)
    {
        msg = in->receive(0);
        ss << owner << " recibi " << Helper::msgToString(msg.message) << " de " << msg.type << std::endl;
        Helper::output(stdout, ss);
        pid = fork();
        if (pid < 0)
        {
            perror("Registro: fork");
        } else if (pid == 0)
        {
            switch (msg.message)
            {
                case QUIERO_LEER:
                    msg.message = LEE;
                    msg.cotizacion = leer();
                    break;
                case QUIERO_ESCRIBIR:
                    msg.message = ESCRIBI;
                    escribir(msg.cotizacion);
                    break;
                default:
                    ss << owner << BG_RED << WHITE << " Error" << NORMAL << " mensaje incorrecto " << Helper::msgToString(msg.message) << " de " << msg.type << std::endl;
                    Helper::output(stderr, ss);
                    exit(EXIT_FAILURE);
            }
            out->send(msg);
            ss << owner << " envie " << Helper::msgToString(msg.message) << " a " << msg.type << std::endl;
            Helper::output(stderr, ss, CYAN);
            exit(EXIT_SUCCESS);
        }
    }
}

double Registro::leer()
{
    double cotizacion;
    mutex->wait();
    if (registro->escritorEscribiendo || (registro->escritoresEsperando && registro->lectores > UMBRAL))
    {
        registro->lectoresEsperando++;
        mutex->post();
        esperaLectores->wait();
        mutex->wait();
        registro->lectoresEsperando--;
    }
    // El primer lector en entrar toma el semaforo de acceso a cotizaciones.
    if (registro->lectoresLeyendo == 0)
    {
        mutex->post();
        cotiz->wait();
        mutex->wait();
    }
    registro->lectoresLeyendo++;
    registro->lectores++;
    // Destrabar a otro lector (en cadena hasta superar el umbral o no haya mas esperando).
    if (registro->lectoresEsperando && (registro->lectores < UMBRAL || registro->escritoresEsperando == 0))
    {
        esperaLectores->post();
    }
    mutex->post();
    cotizacion = registro->cotizacion;
    // Simular lectura.
    Helper::doSleep(lecMin, lecMax);
    mutex->wait();
    registro->lectoresLeyendo--;
    // El ultimo lector libera el semaforo de acceso a cotizaciones.
    if (registro->lectoresLeyendo == 0)
    {
        // Ya que soy el ultimo, si hay un escritor esperando deberia entrar.
        if (registro->escritoresEsperando)
        {
            esperaEscritores->post();
        }
        cotiz->post();
    }
    mutex->post();
    return cotizacion;
}

void Registro::escribir(double cotizacion)
{
    mutex->wait();
    if (registro->escritorEscribiendo || registro->lectoresLeyendo)
    {
        registro->escritoresEsperando++;
        mutex->post();
        esperaEscritores->wait();
        mutex->wait();
        registro->escritoresEsperando--;
    }
    mutex->post();
    cotiz->wait();
    registro->cotizacion = cotizacion;
    // Simular escritura.
    Helper::doSleep(escMin, escMax);
    cotiz->post();
    mutex->wait();
    registro->lectores = 0;
    if (registro->lectoresEsperando)
    {
        esperaLectores->post();
    } else if (registro->escritoresEsperando)
    {
        esperaEscritores->post();
    }
    mutex->post();
}
