#include <sstream>
#include <string>
#include "Almacen.h"
#include "Helper.h"
#include "Config.h"

Almacen::Almacen()
{
    this->owner = "Almacen";
    in = new Queue<struct iMessage>(PATH, Q_FROM_NET_TO_COMPONENT, owner);
    in->get();
    out = new Queue<struct iMessage>(PATH, Q_FROM_COMPONENT_TO_NET, owner);
    out->get();
    qAlmacen = new Queue<struct msgAlmacen>(PATH, Q_ALMACEN, owner);
    qAlmacen->get();
}

void Almacen::init()
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
                case PRODUCIR_ORDEN:
                    producirOrden(msg.data.orden);
                    break;
                case CONSUMIR_ORDEN:
                    msg.message = CONSUMISTE_ORDEN;
                    msg.data.orden = consumirOrden(msg.data.tipoEncargado);
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

struct orden Almacen::consumirOrden(encargados e)
{
    struct msgAlmacen msg;
    long mtype = M_CONS + (long) e;
    msg = qAlmacen->receive(mtype);
    return msg.orden;
}

void Almacen::producirOrden(orden orden)
{
    struct msgAlmacen msg;
    msg.orden = orden;
    for (long mtype = M_CONS; mtype <= M_CONS + CANT_CONSUMIDORES; mtype++)
    {
        msg.type = mtype;
        qAlmacen->send(msg);
    }
}