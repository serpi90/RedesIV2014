#ifndef REGISTRO_H
#define	REGISTRO_H

#include "Queue.cpp"
#include "includes.h"
#include <string>

class Almacen {
public:
    Almacen();
    void init(void);
private:
    void producirOrden(struct orden orden);
    struct orden consumirOrden(enum encargados e);
    std::string owner;
    Queue<struct iMessage> * in, * out;
    Queue<struct msgAlmacen> * qAlmacen;
};

#endif	/* REGISTRO_H */

