#include "iProductor.h"
#include "includes.h"
#include "Queue.cpp"
#include "Helper.h"
#include <sstream>

iProductor::iProductor()
{
    std::stringstream ss;
    struct iMessage msg;
    ss << "iProductor ";
    this->owner = ss.str();

    toNet = new Queue<struct iMessage>(PATH, Q_FROM_INTERFACE_TO_CTL, owner);
    toNet->get();
    fromNet = new Queue<struct iMessage>(PATH, Q_FROM_NET_TO_INTERFACE, owner);
    fromNet->get();

    msg.mtype = M_PROD;
    msg.query.query = REGISTER_PRODUCER;
    toNet->send(msg);
    msg = fromNet->receive(M_PROD);
    this->id = msg.query.id;
}

void iProductor::producirOrden(struct orden orden)
{

    struct iMessage msg;
    long consumidores[CANT_CONSUMIDORES];
    bzero(consumidores, sizeof (consumidores));
    // Preguntar a quienes hay que enviar las ordenes.
    msg.mtype = M_PROD;
    msg.query.id = id;
    msg.query.query = QUERY_CONSUMMERS;
    toNet->send(msg);
    msg = fromNet->receive(msg.query.id);
    memcpy(consumidores, msg.query.consumidores, sizeof (consumidores));

    msg.mtype = M_CONS;
    msg.orden = orden;

    for (unsigned i = 0; i < CANT_CONSUMIDORES; i++)
    {
        if (consumidores[i] == 0)
        {
            Helper::output(stderr, "No hay consumidores suficientes.", RED);
            return;
        }
    }
    for (unsigned i = 0; i < CANT_CONSUMIDORES; i++)
    {
        msg.mtype = consumidores[i];
        if (msg.mtype)
        {
            toNet->send(msg);
        }
    }
}
