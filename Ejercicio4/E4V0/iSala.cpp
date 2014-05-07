#include "iSala.h"

iSala::iSala(bool abajo)
{
    std::stringstream ss;
    this->abajo = abajo;
    ss << "iSala de " << (abajo ? "abajo" : "arriba");
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
    msg.type = abajo ? M_SALA_ABAJO : M_SALA_ARRIBA;
    msg.message = abajo ? ESPERAR_PERSONA_ABAJO : ESPERAR_PERSONA_ARRIBA;
    msg.sender = msg.type;
    out->send(&msg);
    //ss << owner << " Enviando " << Helper::msgToString(msg.message) << std::endl;
    //Helper::output(stdout, ss);
    in->receive(&msg, 0);
    //ss << owner << " Recibi " << Helper::msgToString(msg.message) << std::endl;
    //Helper::output(stdout, ss);
}
