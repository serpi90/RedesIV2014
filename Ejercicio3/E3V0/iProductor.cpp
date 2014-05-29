#include "iProductor.h"
#include "includes.h"
#include <sstream>

iProductor::iProductor(long numero)
{
    std::stringstream ss;
    this->numero = numero;
    this->id = M_PROD + numero;
    ss << "iProductor " << this->numero << " (" << this->id << ")";
    this->owner = ss.str();
    interfaz = new Interfaz(PATH, Q_TO_INTERFACE, Q_FROM_INTERFACE, this->owner);
}

void iProductor::producirOrden(struct orden orden)
{
    struct iMessage msg;
    msg.type = this->id;
    msg.message = PRODUCIR_ORDEN;
    msg.data.orden = orden;
    interfaz->send(msg);
}
