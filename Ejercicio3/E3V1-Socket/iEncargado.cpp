#include "iEncargado.h"
#include "includes.h"
#include "Queue.cpp"
#include "net-idManagerProtocol.h"
#include "Config.h"
#include "Socket.h"
#include <sstream>

iEncargado::iEncargado(long numero)
{
    std::stringstream ss;
    this->numero = numero;
    ss << "iEncargado " << this->numero;

    Config cfg("network.conf");

    unsigned short port = (unsigned short) cfg.getInt("id manager port", 6111);
    std::string address = cfg.getString("id manager address", "localhost");

    struct idManagerMessage query;
    Socket * connection = new Socket(ss.str());
    connection->active(address, port);
    query.type = REGISTER;
    connection->send((char*) &query, sizeof (query));
    connection->receive((char*) &query, sizeof (query));
    connection->doClose();

    this->id = query.mtype.mtype;
    ss << " (" << this->id << ")";
    this->owner = ss.str();
    q = new Queue<struct msgAlmacen>(PATH, Q_FROM_NET_TO_INTERFACE, owner);
    q->get();
}

struct orden iEncargado::consumirOrden()
{
    struct msgAlmacen msg;
    msg = q->receive(this->id);
    return msg.orden;
}
