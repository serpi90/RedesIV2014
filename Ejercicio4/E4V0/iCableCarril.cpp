#include "iCableCarril.h"

iCableCarril::iCableCarril()
{
    std::stringstream ss;
    this->id = M_CABLE_CARRIL;
    ss << "iCableCarril";
    owner = ss.str();
    in = new Queue<struct iMessage>(PATH, Q_CC_TO_INTERFACE, owner);
    in->get();
    out = new Queue<struct iMessage>(PATH, Q_CC_FROM_INTERFACE, owner);
    out->get();
}

void iCableCarril::estoy(bool abajo, long personas[CC_SIZE])
{
    std::stringstream ss;
    int i;
    struct iMessage msg;
    msg.type = M_CABLE_CARRIL;
    msg.sender = this->id;
    msg.message = abajo ? ESTOY_ABAJO : ESTOY_ARRIBA;
    for (i = 0; personas[i] != 0 && i < CC_SIZE; i++)
    {
        msg.data.people[i] = personas[i];
    }
    if (i != CC_SIZE)
    {
        msg.data.people[i] = 0;
    }
    out->send(&msg);
    //ss << owner << " Enviando " << Helper::msgToString(msg.message) << " con " << i << " personas" << std::endl;
    //Helper::output(stdout, ss);
    in->receive(&msg, this->id);
    //ss << owner << " Recibi " << Helper::msgToString(msg.message) << std::endl;
    //Helper::output(stdout, ss);
}

void iCableCarril::cargarPersonas(bool abajo, long personas[CC_SIZE])
{
    std::stringstream ss;
    struct iMessage msg;
    msg.type = M_CABLE_CARRIL;
    msg.sender = this->id;
    msg.message = abajo ? CARGAR_PERSONAS_ABAJO : CARGAR_PERSONAS_ARRIBA;
    //ss << owner << " enviando " << Helper::msgToString(msg.message) << std::endl;
    //Helper::output(stdout, ss);
    out->send(&msg);
    in->receive(&msg, this->id);
    //ss << owner << " recibi " << Helper::msgToString(msg.message) << std::endl;
    //Helper::output(stdout, ss);

    for (int i = 0; i < CC_SIZE; i++)
    {
        personas[i] = msg.data.people[i];
    }
}
