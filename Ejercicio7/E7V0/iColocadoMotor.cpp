#include "iColocadoMotor.h"
#include "Helper.h"
#include <sstream>

iColocadoMotor::iColocadoMotor()
{
    this->owner = "iColocadoMotor";
    in = new Queue<struct iMessage>(PATH, Q_TO_INTERFACE, owner);
    in->get();
    out = new Queue<struct iMessage>(PATH, Q_FROM_INTERFACE, owner);
    out->get();
}

void iColocadoMotor::esperarSecado()
{
    struct iMessage msg;
    msg.type = M_COLOCADO_MOTOR;
    msg.message = RECIBIR_SECADO;
    out->send(msg);
    msg = in->receive(M_COLOCADO_MOTOR);
    if (msg.message != RECIBIR_SECADO_OK)
    {
        std::stringstream ss;
        ss << owner << BG_RED << WHITE << " Error" << NORMAL << " mensaje incorrecto " << Helper::msgToString(msg.message) << " esperaba " << Helper::msgToString(RECIBIR_SECADO_OK) << std::endl;
        Helper::output(stderr, ss);
    }
}

void iColocadoMotor::recibidoSecado()
{
    struct iMessage msg;
    msg.type = M_COLOCADO_MOTOR;
    msg.message = RECIBIDO_SECADO;
    out->send(msg);
}

void iColocadoMotor::entregarMotor()
{
    struct iMessage msg;
    msg.type = M_COLOCADO_MOTOR;
    msg.message = ENTREGAR_MOTOR;
    out->send(msg);
    msg = in->receive(M_COLOCADO_MOTOR);
    if (msg.message != ENTREGAR_MOTOR_OK)
    {
        std::stringstream ss;
        ss << owner << BG_RED << WHITE << " Error" << NORMAL << " mensaje incorrecto " << Helper::msgToString(msg.message) << " esperaba " << Helper::msgToString(ENTREGAR_MOTOR_OK) << std::endl;
        Helper::output(stderr, ss);
    }
}