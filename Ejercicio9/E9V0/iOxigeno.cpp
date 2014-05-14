#include "iOxigeno.h"
#include "includes.h"
#include <sstream>

iOxigeno::iOxigeno(long numero)
{
    std::stringstream ss;
    this->numero = numero;
    this->id = numero * 2 + 1;
    ss << "iOxigeno " << this->numero << " (" << this->id << ")";
    this->owner = ss.str();
    interfaz = new Interfaz(PATH, Q_TO_INTERFACE, Q_FROM_INTERFACE, this->owner);
}

void iOxigeno::activar()
{
    struct iMessage msg;
    msg.type = this->id;
    msg.kind = OXIGENO;
    msg.message = ACTIVAR;
    interfaz->request(msg, this->id, ACTIVADO);
}

void iOxigeno::terminar()
{
    struct iMessage msg;
    msg.type = this->id;
    msg.kind = OXIGENO;
    msg.message = TERMINAR;
    interfaz->request(msg, this->id, TERMINADO);
}
