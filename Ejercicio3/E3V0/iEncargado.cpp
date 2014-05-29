#include "iEncargado.h"
#include "includes.h"
#include <sstream>

iEncargado::iEncargado(long numero, enum encargados e)
{
    std::stringstream ss;
    this->numero = numero;
    this->id = M_CONS + numero;
    ss << "iEncargado " << this->numero << " (" << this->id << ")";
    this->owner = ss.str();
    interfaz = new Interfaz(PATH, Q_TO_INTERFACE, Q_FROM_INTERFACE, this->owner);
    this->e = e;
}

struct orden iEncargado::consumirOrden()
{
    struct iMessage msg;
    msg.type = this->id;
    msg.message = CONSUMIR_ORDEN;
    msg.data.tipoEncargado = e;
    msg = interfaz->request(msg, this->id, CONSUMISTE_ORDEN);
    return msg.data.orden;
}
