#include "iEncargado.h"
#include "includes.h"
#include "Queue.cpp"
#include <sstream>

iEncargado::iEncargado(long numero)
{
    std::stringstream ss;
    this->numero = numero;
    this->id = M_CONS + numero;
    ss << "iEncargado " << this->numero << " (" << this->id << ")";
    this->owner = ss.str();
    q = new Queue<struct msgAlmacen>(PATH, Q_ALMACEN, owner);
    q->get();
}

struct orden iEncargado::consumirOrden()
{
    struct msgAlmacen msg;
    msg = q->receive(this->id);
    return msg.orden;
}
