#ifndef IPINTADO_H
#define	IPINTADO_H
#include "Queue.cpp"
#include "includes.h"
#include <string>

class iPintado {
public:
    iPintado();
    void esperarChasis();
    void recibidoChasis();
    void entregarPintado();
    unsigned reponer();
private:
    std::string owner;
    Queue<struct iMessage> * in, * out;
};

#endif	/* IPINTADO_H */

