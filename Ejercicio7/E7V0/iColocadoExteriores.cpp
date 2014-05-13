#include "iColocadoExteriores.h"
#include "Helper.h"
#include <sstream>

iColocadoExteriores::iColocadoExteriores()
{
    this->owner = "iColocadoExteriores";
    in = new Queue<struct iMessage>(PATH, Q_TO_INTERFACE, owner);
    in->get();
    out = new Queue<struct iMessage>(PATH, Q_FROM_INTERFACE, owner);
    out->get();
}

void iColocadoExteriores::esperarInteriores()
{
    struct iMessage msg;
    msg.type = M_COLOCADO_EXTERIOR;
    msg.message = RECIBIR_INTERIORES;
    out->send(msg);
    msg = in->receive(M_COLOCADO_EXTERIOR);
    if (msg.message != RECIBIR_INTERIORES_OK)
    {
        std::stringstream ss;
        ss << owner << BG_RED << WHITE << " Error" << NORMAL << " mensaje incorrecto " << Helper::msgToString(msg.message) << " esperaba " << Helper::msgToString(RECIBIR_INTERIORES_OK) << std::endl;
        Helper::output(stderr, ss);
    }
}

void iColocadoExteriores::recibidoInteriores()
{
    struct iMessage msg;
    msg.type = M_COLOCADO_EXTERIOR;
    msg.message = RECIBIDO_INTERIORES;
    out->send(msg);
}

unsigned iColocadoExteriores::reponerBaules()
{
    struct iMessage msg;
    msg.type = M_COLOCADO_EXTERIOR;
    msg.message = REPONER;
    msg.data.aReponer = BAUL;
    out->send(msg);
    msg = in->receive(M_COLOCADO_EXTERIOR);
    if (msg.message != REPONER_OK)
    {
        std::stringstream ss;
        ss << owner << BG_RED << WHITE << " Error" << NORMAL << " mensaje incorrecto " << Helper::msgToString(msg.message) << " esperaba " << Helper::msgToString(REPONER_OK) << std::endl;
        Helper::output(stderr, ss);
    }
    return msg.data.cantidad;
}

unsigned iColocadoExteriores::reponerCapos()
{
    struct iMessage msg;
    msg.type = M_COLOCADO_EXTERIOR;
    msg.message = REPONER;
    msg.data.aReponer = CAPO;
    out->send(msg);
    msg = in->receive(M_COLOCADO_EXTERIOR);
    if (msg.message != REPONER_OK)
    {
        std::stringstream ss;
        ss << owner << BG_RED << WHITE << " Error" << NORMAL << " mensaje incorrecto " << Helper::msgToString(msg.message) << " esperaba " << Helper::msgToString(REPONER_OK) << std::endl;
        Helper::output(stderr, ss);
    }
    return msg.data.cantidad;
}

unsigned iColocadoExteriores::reponerPuertas()
{
    struct iMessage msg;
    msg.type = M_COLOCADO_EXTERIOR;
    msg.message = REPONER;
    msg.data.aReponer = PUERTA;
    out->send(msg);
    msg = in->receive(M_COLOCADO_EXTERIOR);
    if (msg.message != REPONER_OK)
    {
        std::stringstream ss;
        ss << owner << BG_RED << WHITE << " Error" << NORMAL << " mensaje incorrecto " << Helper::msgToString(msg.message) << " esperaba " << Helper::msgToString(REPONER_OK) << std::endl;
        Helper::output(stderr, ss);
    }
    return msg.data.cantidad;
}