#ifndef IPRODUCTOR_H
#define	IPRODUCTOR_H
#include "Queue.cpp"
#include "Socket.h"
#include <string>

class iProductor {
public:
    iProductor();
    void producirOrden(struct orden orden);
private:
    std::string owner;
    Queue<struct msgAlmacen> * q;
    Socket * connection;
};

#endif	/* IPRODUCTOR_H */

