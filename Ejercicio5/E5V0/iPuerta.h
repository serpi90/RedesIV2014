#ifndef IPUERTA_H
#define	IPUERTA_H

#include "includes.h"
#include "Queue.cpp"
#include "Helper.h"

class iPuerta {
public:
    iPuerta(enum location ubicacion, long number);
    void esperarPersona();
private:
    Queue<struct iMessage> * in, * out;
    enum location ubicacion;
    std::string owner;
    long number;
    long id;
};

#endif	/* IPUERTA_H */
