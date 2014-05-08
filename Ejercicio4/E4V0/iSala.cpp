#include "iSala.h"

iSala::iSala(enum location ubicacion)
{
    std::stringstream ss;
    this->ubicacion = ubicacion;
    ss << "iSala de " << (ubicacion == BOTTOM ? "abajo" : "arriba");
    owner = ss.str();
    in = new Queue<struct iMessage>(PATH, Q_SALA_TO_INTERFACE, owner);
    in->get();
    out = new Queue<struct iMessage>(PATH, Q_SALA_FROM_INTERFACE, owner);
    out->get();
}

void iSala::esperarPersona()
{
    std::stringstream ss;
    struct iMessage msg;
    long id = ubicacion == BOTTOM ? M_SALA_ABAJO : M_SALA_ARRIBA;
    msg.type = id;
    msg.message = ubicacion == BOTTOM ? ESPERAR_PERSONA_ABAJO : ESPERAR_PERSONA_ARRIBA;
    msg.sender = msg.type;
    out->send(&msg);
    ss << owner << " Enviando " << Helper::msgToString(msg.message) << std::endl;
    Helper::output(stdout, ss);
    in->receive(&msg, id);
    ss << owner << " Recibi " << Helper::msgToString(msg.message) << std::endl;
    Helper::output(stdout, ss);
}
