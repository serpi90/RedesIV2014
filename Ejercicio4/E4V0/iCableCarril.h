#ifndef ICABLECARRIL_H
#define	ICABLECARRIL_H

#include "Queue.cpp"
#include "includes.h"
#include "Helper.h"

class iCableCarril {
public:
    iCableCarril();
    void estoy(bool abajo, long personas[CC_SIZE]);
    void cargarPersonas(bool abajo, long personas[CC_SIZE]);
private:
    Queue<struct iMessage> * in, * out;
    int id;
    std::string owner;
};

#endif	/* ICABLECARRIL_H */