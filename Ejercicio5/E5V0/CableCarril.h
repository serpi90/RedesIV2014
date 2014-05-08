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
    void llegar(enum location location, long numero, long personas[CC_SIZE]);
    void cargarPersonas(enum location ubicacion, long numero, long personas[CC_SIZE]);
private:
    Queue<struct iMessage> * in, * out;
    Queue<struct personMessage>* persona, * cc;
    SharedMemory<struct registro> * shm;
    Semaphore * mutex;
    SemaphoreArray * llenaAbajo;
    SemaphoreArray * llenaArriba;
    SemaphoreArray * vacia;
    std::string owner;
    struct registro * registro;
};

#endif	/* CABLECARRIL_H */
