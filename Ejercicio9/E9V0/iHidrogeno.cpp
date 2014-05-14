#include "iHidrogeno.h"
#include "includes.h"
#include <sstream>

iHidrogeno::iHidrogeno(long numero)
{
    std::stringstream ss;
    this->numero = numero;
    this->id = numero * 2 + 2;
    ss << "iHidrogeno " << this->numero << " (" << this->id << ")";
    this->owner = ss.str();
    interfaz = new Interfaz(PATH, Q_TO_INTERFACE, Q_FROM_INTERFACE, this->owner);
}

void iHidrogeno::activar()
{
    struct iMessage msg;
    msg.type = this->id;
    msg.kind = HIDROGENO;
    msg.message = ACTIVAR;
    interfaz->request(msg, this->id, ACTIVADO);
}

void iHidrogeno::terminar()
{
    struct iMessage msg;
    msg.type = this->id;
    msg.kind = HIDROGENO;
    msg.message = TERMINAR;
    interfaz->request(msg, this->id, TERMINADO);
}
