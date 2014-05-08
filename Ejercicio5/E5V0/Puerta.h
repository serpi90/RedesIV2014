#ifndef PUERTA_H
#define	PUERTA_H

#include "includes.h"
#include "Helper.h"
#include "Queue.cpp"
#include "Semaphore.h"
#include "SharedMemory.cpp"
#include <sstream>
#include <string>

class Puerta {
public:
    Puerta();
    void init();
private:
    Queue<struct iMessage> * in, * out;
    Queue<struct personMessage>* persona, * salaAbajo, * salaArriba;
    SharedMemory<struct registro> * shm;
    Semaphore * mutex;
    SemaphoreArray * llenaAbajo;
    SemaphoreArray * llenaArriba;
    SemaphoreArray * vacia;
    std::string owner;
    struct registro * registro;
    void esperarPersona(enum location ubicacion, long numero);
};

#endif	/* PUERTA_H */
