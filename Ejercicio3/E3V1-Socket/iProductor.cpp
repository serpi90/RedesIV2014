#include "net-idManagerProtocol.h"
#include "iProductor.h"
#include "includes.h"
#include "Queue.cpp"
#include "Config.h"
#include <sstream>

iProductor::iProductor()
{
    std::stringstream ss;
    ss << "iProductor ";
    this->owner = ss.str();
    q = new Queue<struct msgAlmacen>(PATH, Q_FROM_INTERFACE_TO_CTL, owner);
    q->get();
    Config cfg("network.conf");

    unsigned short port = (unsigned short) cfg.getInt("id manager port", 6111);
    std::string address = cfg.getString("id manager address", "localhost");

    connection = new Socket(owner);
    connection->active(address, port);
}

void iProductor::producirOrden(struct orden orden)
{

    struct msgAlmacen msg;
    msg.orden = orden;
    struct idManagerMessage query;
    query.type = QUERY;
    connection->send((char*) &query, sizeof (query));

    do
    {
        connection->receive((char*) &query, sizeof (query));
        msg.type = query.mtype.mtype;
        if (msg.type)
        {
            q->send(msg);
        }
    } while (query.mtype.more);
}
