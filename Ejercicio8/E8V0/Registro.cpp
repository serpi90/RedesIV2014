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
    sems = new Semaphore(PATH, SEM_EMBOTELLADORAS, owner);
    sems->get();
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
        ss << owner << " recibi " << Helper::msgToString(msg.message) << " de " << Helper::msgToString(msg.type) << std::endl;
        Helper::output(stdout, ss);
        pid = fork();
        if (pid < 0)
        {
            perror("Registro: fork");
        } else if (pid == 0)
        {
            switch (msg.message)
            {
                case TERMINE:
                    msg.message = CONTINUAR;
                    termine();
                    break;
                default:
                    ss << owner << BG_RED << WHITE << " Error" << NORMAL << " mensaje incorrecto " << Helper::msgToString(msg.message) << " de " << Helper::msgToString(msg.type) << std::endl;
                    Helper::output(stderr, ss);
                    exit(EXIT_FAILURE);
            }
            out->send(msg);
            ss << owner << " envie " << Helper::msgToString(msg.message) << " a " << Helper::msgToString(msg.type) << std::endl;
            Helper::output(stderr, ss, CYAN);
            exit(EXIT_SUCCESS);
        }
    }
}

void Registro::termine()
{
    std::stringstream ss;
    mutex->wait();
    registro->terminaron++;
    ss << owner << " terminamos " << registro->terminaron << "/" << CANTIDAD_EMBOTELLADORAS << std::endl;
    Helper::output(stdout, ss, PURPLE);

    if (registro->terminaron < CANTIDAD_EMBOTELLADORAS)
    {
        mutex->post();
        sems->wait();
    } else
    {
        registro->terminaron = 0;
        mutex->post();
        for (unsigned e = 1; e < CANTIDAD_EMBOTELLADORAS; e++)
        {
            sems->post();
        }
    }
}