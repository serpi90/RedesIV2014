#ifndef ICABLECARRIL_H
#define	ICABLECARRIL_H

#include "Queue.cpp"
#include "includes.h"
#include "Helper.h"

class iCableCarril {
public:
    iCableCarril(long number);
    void estoy(enum location ubicacion, long personas[CC_SIZE]);
    void cargarPersonas(enum location ubicacion, long personas[CC_SIZE]);
private:
    Queue<struct iMessage> * in, * out;
    int id;
    std::string owner;
    long number;
};

#endif	/* ICABLECARRIL_H */