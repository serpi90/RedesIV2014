#include "iPintado.h"
#include "Helper.h"
#include <sstream>

iPintado::iPintado()
{
    this->owner = "iPintado";
    in = new Queue<struct iMessage>(PATH, Q_TO_INTERFACE, owner);
    in->get();
    out = new Queue<struct iMessage>(PATH, Q_FROM_INTERFACE, owner);
    out->get();
}

void iPintado::esperarChasis()
{
    struct iMessage msg;
    msg.type = M_PINTADO;
    msg.message = RECIBIR_CHASIS;
    out->send(msg);
    msg = in->receive(M_PINTADO);
    if (msg.message != RECIBIR_CHASIS_OK)
    {
        std::stringstream ss;
        ss << owner << BG_RED << WHITE << " Error" << NORMAL << " mensaje incorrecto " << Helper::msgToString(msg.message) << " esperaba " << Helper::msgToString(RECIBIR_CHASIS_OK) << std::endl;
        Helper::output(stderr, ss);
    }
}

void iPintado::recibidoChasis()
{
    struct iMessage msg;
    msg.type = M_PINTADO;
    msg.message = RECIBIDO_CHASIS;
    out->send(msg);
}

void iPintado::entregarPintado()
{
    struct iMessage msg;
    msg.type = M_PINTADO;
    msg.message = ENTREGAR_PINTADO;
    out->send(msg);
    msg = in->receive(M_PINTADO);
    if (msg.message != ENTREGAR_PINTADO_OK)
    {
        std::stringstream ss;
        ss << owner << BG_RED << WHITE << " Error" << NORMAL << " mensaje incorrecto " << Helper::msgToString(msg.message) << " esperaba " << Helper::msgToString(RECIBIR_CHASIS_OK) << std::endl;
        Helper::output(stderr, ss);
    }
}

unsigned iPintado::reponer()
{
    struct iMessage msg;
    msg.type = M_PINTADO;
    msg.message = REPONER;
    msg.data.aReponer = PINTURA;
    out->send(msg);
    msg = in->receive(M_PINTADO);
    if (msg.message != REPONER_OK)
    {
        std::stringstream ss;
        ss << owner << BG_RED << WHITE << " Error" << NORMAL << " mensaje incorrecto " << Helper::msgToString(msg.message) << " esperaba " << Helper::msgToString(REPONER_OK) << std::endl;
        Helper::output(stderr, ss);
    }
    return msg.data.cantidad;
}
