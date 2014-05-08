#ifndef ISALA_H
#define	ISALA_H

#include "includes.h"
#include "Queue.cpp"
#include "Helper.h"

class iSala {
public:
    iSala(enum location ubicacion);
    void esperarPersona();
private:
    Queue<struct iMessage> * in, * out;
    enum location ubicacion;
    std::string owner;
};

#endif	/* ISALA_H */
