#include "iPuerta.h"

iPuerta::iPuerta(enum location ubicacion, long number)
{
    std::stringstream ss;
    this->ubicacion = ubicacion;
    this->id = (ubicacion == ABAJO ? M_PUERTA_ABAJO : M_PUERTA_ARRIBA) + number;
    this->number = number;

    ss << "iPuerta de " << Helper::msgToString(ubicacion) << " " << number << " (" << id << ")";
    owner = ss.str();
    in = new Queue<struct iMessage>(PATH, Q_PUERTA_TO_INTERFACE, owner);
    in->get();
    out = new Queue<struct iMessage>(PATH, Q_PUERTA_FROM_INTERFACE, owner);
    out->get();
}

void iPuerta::esperarPersona()
{
    std::stringstream ss;
    struct iMessage msg;
    msg.type = id;
    msg.sender = id;
    msg.data.loc = ubicacion;
    msg.message = ubicacion == ABAJO ? ESPERAR_PERSONA_ABAJO : ESPERAR_PERSONA_ARRIBA;
    msg.number = number;
    out->send(&msg);
    ss << owner << " Enviando " << Helper::msgToString(msg.message) << std::endl;
    Helper::output(stdout, ss);
    in->receive(&msg, id);
    ss << owner << " Recibi " << Helper::msgToString(msg.message) << std::endl;
    Helper::output(stdout, ss);
}
