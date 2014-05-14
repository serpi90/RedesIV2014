
#include "includes.h"

#include "Registro.h"
#include <sstream>
#include <string>
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
    semH = new Semaphore(PATH, SEM_H, owner);
    semH->get();
    semH1 = new Semaphore(PATH, SEM_H1, owner);
    semH1->get();
    semO = new Semaphore(PATH, SEM_O, owner);
    semO->get();
    shm = new SharedMemory<struct registro>(PATH, SHM_REGISTRO, owner);
    shm->get();
    this->registro = shm->attach();
}

void Registro::init()
{
    std::stringstream ss;
    struct iMessage msg;
    pid_t pid;
    while (true)
    {
        msg = in->receive(0);
        ss << owner << " recibi " << Helper::msgToString(msg.message) << " de " << Helper::msgToString(msg.kind) << "(" << msg.type << ")" << std::endl;
        Helper::output(stdout, ss);
        pid = fork();
        if (pid < 0)
        {
            perror("Registro: fork");
        } else if (pid == 0)
        {
            switch (msg.message)
            {
                case ACTIVAR:
                    switch (msg.kind)
                    {
                        case HIDROGENO:
                            activarHidrogeno();
                            break;
                        case OXIGENO:
                            activarOxigeno();
                            break;
                        default:
                            ss << owner << BG_RED << WHITE << " Error" << NORMAL << " mensaje incorrecto " << Helper::msgToString(msg.message) << " de " << Helper::msgToString(msg.kind) << "(" << msg.type << ")" << std::endl;
                            Helper::output(stderr, ss);
                            exit(EXIT_FAILURE);
                    }
                    msg.message = ACTIVADO;
                    break;
                case TERMINAR:
                    switch (msg.kind)
                    {
                        case HIDROGENO:
                            terminarHidrogeno();
                            break;
                        case OXIGENO:
                            terminarOxigeno();
                            break;
                        default:
                            ss << owner << BG_RED << WHITE << " Error" << NORMAL << " mensaje incorrecto " << Helper::msgToString(msg.message) << " de " << Helper::msgToString(msg.kind) << "(" << msg.type << ")" << std::endl;
                            Helper::output(stderr, ss);
                            exit(EXIT_FAILURE);
                    }
                    msg.message = TERMINADO;
                    break;
                default:
                    ss << owner << BG_RED << WHITE << " Error" << NORMAL << " mensaje incorrecto " << Helper::msgToString(msg.message) << " de " << Helper::msgToString(msg.kind) << "(" << msg.type << ")" << std::endl;
                    Helper::output(stderr, ss);
                    exit(EXIT_FAILURE);
            }
            out->send(msg);
            ss << owner << " envie " << Helper::msgToString(msg.message) << " a " << Helper::msgToString(msg.kind) << "(" << msg.type << ")" << std::endl;
            Helper::output(stderr, ss, CYAN);
            exit(EXIT_SUCCESS);
        }
    }
}

void Registro::activarHidrogeno()
{
    std::stringstream ss;
    semH->wait();
    mutex->wait();
    registro->activos++;
    registro->hidrogenos++;
    ss << "H: " << registro->hidrogenos << std::endl;
    ss << "A: " << registro->activos << std::endl;
    Helper::output(stdout, ss, D_GREEN);
    // Si no soy el ultimo en arrancar, despierto a otro hidrogeno
    if (registro->hidrogenos <= CANTIDAD_HIDROGENO - 1)
    {
        semH->post();
    }
    mutex->post();
}

void Registro::activarOxigeno()
{
    semO->wait();
}

void Registro::terminarHidrogeno()
{
    std::stringstream ss;
    mutex->wait();
    registro->activos--;
    // Si soy el ultimo en terminar despierto al oxigeno
    ss << "H: " << registro->hidrogenos << std::endl;
    ss << "A: " << registro->activos << std::endl;
    Helper::output(stdout, ss, GREEN);
    if (registro->hidrogenos == CANTIDAD_HIDROGENO && !registro->activos)
    {
        semO->post();
    }
    mutex->post();
    semH1->wait();
}

void Registro::terminarOxigeno()
{
    std::stringstream ss;
    // Indico a los hidrogenos que terminen.
    mutex->wait();
    while (registro->hidrogenos)
    {
        registro->hidrogenos--;
        semH1->post();
    }
    ss << "H: " << registro->hidrogenos << std::endl;
    ss << "A: " << registro->activos << std::endl;
    Helper::output(stdout, ss, GREEN);
    mutex->post();
    // Arranco la nueva tanda de hidrogenos.
    semH->post();
}