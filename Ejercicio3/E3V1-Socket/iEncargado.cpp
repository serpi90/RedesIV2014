#include "iEncargado.h"
#include "includes.h"
#include "Queue.cpp"
#include <sstream>

iEncargado::iEncargado(enum consummerType tipo) {
    std::stringstream ss;
    struct iMessage msg;
    ss << "iEncargado ";
    this->owner = ss.str();

    fromNet = new Queue<struct iMessage>(PATH, Q_FROM_NET_TO_INTERFACE, owner);
    fromNet->get();
    toNet = new Queue<struct iMessage>(PATH, Q_FROM_INTERFACE_TO_CTL, owner);
    toNet->get();

    msg.mtype = M_CONS;
    msg.query.query = REGISTER_CONSUMMER;
    msg.query.type = tipo;
    toNet->send(msg);
    msg = fromNet->receive(M_CONS);
    this->id = msg.query.id;
}

struct orden iEncargado::consumirOrden() {
    struct iMessage msg;
    msg = fromNet->receive(this->id);
    return msg.orden;
}
