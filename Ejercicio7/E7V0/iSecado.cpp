#include "iSecado.h"
#include "Helper.h"
#include <sstream>

iSecado::iSecado()
{
    this->owner = "iSecado";
    in = new Queue<struct iMessage>(PATH, Q_TO_INTERFACE, owner);
    in->get();
    out = new Queue<struct iMessage>(PATH, Q_FROM_INTERFACE, owner);
    out->get();
}

void iSecado::esperarPintado()
{
    struct iMessage msg;
    msg.type = M_SECADO;
    msg.message = RECIBIR_PINTADO;
    out->send(msg);
    msg = in->receive(M_SECADO);
    if (msg.message != RECIBIR_PINTADO_OK)
    {
        std::stringstream ss;
        ss << owner << BG_RED << WHITE << " Error" << NORMAL << " mensaje incorrecto " << Helper::msgToString(msg.message) << " esperaba " << Helper::msgToString(RECIBIR_PINTADO_OK) << std::endl;
        Helper::output(stderr, ss);
    }
}

void iSecado::recibidoPintado()
{
    struct iMessage msg;
    msg.type = M_SECADO;
    msg.message = RECIBIDO_PINTADO;
    out->send(msg);
}

void iSecado::entregarSecado()
{
    struct iMessage msg;
    msg.type = M_SECADO;
    msg.message = ENTREGAR_SECADO;
    out->send(msg);
    msg = in->receive(M_SECADO);
    if (msg.message != ENTREGAR_SECADO_OK)
    {
        std::stringstream ss;
        ss << owner << BG_RED << WHITE << " Error" << NORMAL << " mensaje incorrecto " << Helper::msgToString(msg.message) << " esperaba " << Helper::msgToString(RECIBIR_PINTADO_OK) << std::endl;
        Helper::output(stderr, ss);
    }
}