#include "iLector.h"
#include "includes.h"
#include <sstream>

iLector::iLector(long numero)
{
    std::stringstream ss;
    this->numero = numero;
    this->id = numero * 2 + 2;
    ss << "iHidrogeno " << this->numero << " (" << this->id << ")";
    this->owner = ss.str();
    interfaz = new Interfaz(PATH, Q_TO_INTERFACE, Q_FROM_INTERFACE, this->owner);
}

double iLector::leer()
{
    struct iMessage msg;
    msg.type = this->id;
    msg.message = QUIERO_LEER;
    interfaz->request(msg, this->id, LEE);
    return msg.cotizacion;
}
