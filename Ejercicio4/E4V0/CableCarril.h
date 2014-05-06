#ifndef CABLECARRIL_H
#define	CABLECARRIL_H
#include "Queue.cpp"
#include "SharedMemory.cpp"
#include "Semaphore.h"
#include "includes.h"
#include "Helper.h"

class CableCarril {
public:
    CableCarril();
    void init();
    void estoy(enum location location, long personas[CC_SIZE]);
    void cargarPersonasAbajo(long personas[CC_SIZE]);
    void cargarPersonasArriba(long personas[CC_SIZE]);
private:
    Queue<struct iMessage> * in, * out;
    Queue<struct personMessage>* persona, * cc;
    SharedMemory<struct cableCarril> * shm;
    Semaphore * mutex;
    SemaphoreArray * llena;
    Semaphore * vacia;
    std::string owner;
    struct cableCarril * cableCarril;
};

#endif	/* CABLECARRIL_H */
