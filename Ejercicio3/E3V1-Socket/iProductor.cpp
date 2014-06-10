#include "iProductor.h"
#include "includes.h"
#include "Queue.cpp"
#include "Helper.h"
#include "net-idManagerProtocol.h"
#include <sstream>

iProductor::iProductor() {
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

void iProductor::producirOrden(struct orden orden) {

    struct iMessage msg;
    struct mtypes consumidores;
    consumidores.disks = 0;
    consumidores.motherboards = 0;
    consumidores.processors = 0;
    // Preguntar a quienes hay que enviar las ordenes.
    msg.mtype = M_PROD;
    msg.query.id = id;
    msg.query.query = GET_CONSUMMERS;
    toNet->send(msg);
    msg = fromNet->receive(msg.query.id);
    consumidores = msg.query.consumidores;

    msg.mtype = M_CONS;
    msg.orden = orden;

    if (consumidores.disks == 0 || consumidores.processors == 0 || consumidores.motherboards == 0) {
        Helper::output(stderr, "No hay consumidores suficientes.", RED);
        return;
    }

    msg.mtype = consumidores.disks;
    toNet->send(msg);
    msg.mtype = consumidores.processors;
    toNet->send(msg);
    msg.mtype = consumidores.motherboards;
    toNet->send(msg);
}
