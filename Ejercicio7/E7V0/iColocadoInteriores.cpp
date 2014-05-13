#include "iColocadoInteriores.h"
#include "Helper.h"
#include <sstream>

iColocadoInteriores::iColocadoInteriores()
{
    this->owner = "iColocadoInteriores";
    in = new Queue<struct iMessage>(PATH, Q_TO_INTERFACE, owner);
    in->get();
    out = new Queue<struct iMessage>(PATH, Q_FROM_INTERFACE, owner);
    out->get();
}

void iColocadoInteriores::esperarMotor()
{
    struct iMessage msg;
    msg.type = M_COLOCADO_INTERIOR;
    msg.message = RECIBIR_MOTOR;
    out->send(msg);
    msg = in->receive(M_COLOCADO_INTERIOR);
    if (msg.message != RECIBIR_MOTOR_OK)
    {
        std::stringstream ss;
        ss << owner << BG_RED << WHITE << " Error" << NORMAL << " mensaje incorrecto " << Helper::msgToString(msg.message) << " esperaba " << Helper::msgToString(RECIBIR_MOTOR_OK) << std::endl;
        Helper::output(stderr, ss);
    }
}

void iColocadoInteriores::recibidoMotor()
{
    struct iMessage msg;
    msg.type = M_COLOCADO_INTERIOR;
    msg.message = RECIBIDO_MOTOR;
    out->send(msg);
}

void iColocadoInteriores::entregarInteriores()
{
    struct iMessage msg;
    msg.type = M_COLOCADO_INTERIOR;
    msg.message = ENTREGAR_INTERIORES;
    out->send(msg);
    msg = in->receive(M_COLOCADO_INTERIOR);
    if (msg.message != ENTREGAR_INTERIORES_OK)
    {
        std::stringstream ss;
        ss << owner << BG_RED << WHITE << " Error" << NORMAL << " mensaje incorrecto " << Helper::msgToString(msg.message) << " esperaba " << Helper::msgToString(ENTREGAR_INTERIORES_OK) << std::endl;
        Helper::output(stderr, ss);
    }
}

unsigned iColocadoInteriores::reponerInteriores()
{
    struct iMessage msg;
    msg.type = M_COLOCADO_INTERIOR;
    msg.message = REPONER;
    msg.data.aReponer = INTERIORES;
    out->send(msg);
    msg = in->receive(M_COLOCADO_INTERIOR);
    if (msg.message != REPONER_OK)
    {
        std::stringstream ss;
        ss << owner << BG_RED << WHITE << " Error" << NORMAL << " mensaje incorrecto " << Helper::msgToString(msg.message) << " esperaba " << Helper::msgToString(REPONER_OK) << std::endl;
        Helper::output(stderr, ss);
    }
    return msg.data.cantidad;
}

unsigned iColocadoInteriores::reponerRuedas()
{
    struct iMessage msg;
    msg.type = M_COLOCADO_INTERIOR;
    msg.message = REPONER;
    msg.data.aReponer = RUEDA;
    out->send(msg);
    msg = in->receive(M_COLOCADO_INTERIOR);
    if (msg.message != REPONER_OK)
    {
        std::stringstream ss;
        ss << owner << BG_RED << WHITE << " Error" << NORMAL << " mensaje incorrecto " << Helper::msgToString(msg.message) << " esperaba " << Helper::msgToString(REPONER_OK) << std::endl;
        Helper::output(stderr, ss);
    }
    return msg.data.cantidad;
}