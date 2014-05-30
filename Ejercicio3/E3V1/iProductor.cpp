#include "iProductor.h"
#include "includes.h"
#include "Queue.cpp"
#include <sstream>

iProductor::iProductor()
{
    std::stringstream ss;
    ss << "iProductor ";
    this->owner = ss.str();
    q = new Queue<struct msgAlmacen>(PATH, Q_FROM_INTERFACE_TO_NET, owner);
    q->get();
}

void iProductor::producirOrden(struct orden orden)
{
    struct msgAlmacen msg;
    msg.orden = orden;
    for (long mtype = M_CONS; mtype < M_CONS + CANT_CONSUMIDORES; mtype++)
    {
        msg.type = mtype;
        q->send(msg);
    }
}
