#include "iCableCarril.h"

iCableCarril::iCableCarril(long number)
{
    std::stringstream ss;
    this->number = number;
    id = M_CABLE_CARRIL + number;
    ss << "iCableCarril " << number << " (" << id << ")";
    owner = ss.str();
    in = new Queue<struct iMessage>(PATH, Q_CC_TO_INTERFACE, owner);
    in->get();
    out = new Queue<struct iMessage>(PATH, Q_CC_FROM_INTERFACE, owner);
    out->get();
}

void iCableCarril::estoy(enum location ubicacion, long personas[CC_SIZE])
{
    std::stringstream ss;
    int i;
    struct iMessage msg;
    msg.type = id;
    msg.sender = id;
    msg.number = this->number;
    msg.message = ubicacion == ABAJO ? ESTOY_ABAJO : ESTOY_ARRIBA;
    for (i = 0; personas[i] != 0 && i < CC_SIZE; i++)
    {
        msg.data.people[i] = personas[i];
    }
    if (i != CC_SIZE)
    {
        msg.data.people[i] = 0;
    }
    out->send(&msg);
    ss << owner << " Enviando " << Helper::msgToString(msg.message) << " con " << i << " personas" << std::endl;
    Helper::output(stdout, ss);
    in->receive(&msg, id);
    ss << owner << " Recibi " << Helper::msgToString(msg.message) << std::endl;
    Helper::output(stdout, ss);
}

void iCableCarril::cargarPersonas(enum location ubicacion, long personas[CC_SIZE])
{
    std::stringstream ss;
    struct iMessage msg;
    msg.type = M_CABLE_CARRIL;
    msg.sender = this->id;
    msg.message = ubicacion == ABAJO ? CARGAR_PERSONAS_ABAJO : CARGAR_PERSONAS_ARRIBA;
    msg.number = this->number;
    ss << owner << " enviando " << Helper::msgToString(msg.message) << std::endl;
    Helper::output(stdout, ss);
    out->send(&msg);
    in->receive(&msg, this->id);
    ss << owner << " recibi " << Helper::msgToString(msg.message) << std::endl;
    Helper::output(stdout, ss);

    for (int i = 0; i < CC_SIZE; i++)
    {
        personas[i] = msg.data.people[i];
    }
}
