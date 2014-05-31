#ifndef IPRODUCTOR_H
#define	IPRODUCTOR_H
#include "Queue.cpp"
#include "Socket.h"
#include <string>
#include "RPC/idManager.h"

class iProductor {
public:
    iProductor();
    void producirOrden(struct orden orden);
private:
    std::string owner;
    Queue<struct msgAlmacen> * q;
    CLIENT *clnt;
};

#endif	/* IPRODUCTOR_H */

