#ifndef SALA_H
#define	SALA_H

#include "includes.h"
#include "Helper.h"
#include "Queue.cpp"
#include "Semaphore.h"
#include "SharedMemory.cpp"
#include <sstream>
#include <string>

class Sala {
public:
    Sala();
    void init();
private:
    Queue<struct iMessage> * in, * out;
    Queue<struct personMessage>* persona, * salaAbajo, * salaArriba;
    SharedMemory<struct cableCarril> * shm;
    Semaphore * mutex;
    SemaphoreArray * llena;
    Semaphore * vacia;
    std::string owner;
    struct cableCarril * cableCarril;
    bool abajo;

    void ingresarPersonaAbajo();
    void ingresarPersonaArriba();
};

#endif	/* SALA_H */
