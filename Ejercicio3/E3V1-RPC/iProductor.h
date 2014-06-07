#ifndef IPRODUCTOR_H
#define	IPRODUCTOR_H
#include "Queue.cpp"
#include "includes.h"
#include <string>

class iProductor {
public:
    iProductor();
    void producirOrden(struct orden orden);
private:
    long id;
    std::string owner;
    Queue<struct iMessage> * toNet;
    Queue<struct iMessage> * fromNet;
};

#endif	/* IPRODUCTOR_H */

