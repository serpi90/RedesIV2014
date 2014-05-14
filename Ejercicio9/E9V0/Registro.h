#ifndef REGISTRO_H
#define	REGISTRO_H

#include "Queue.cpp"
#include "SharedMemory.cpp"
#include "Semaphore.h"
#include "includes.h"
#include <string>

class Registro {
public:
    Registro();
    void init(void);
private:
    std::string owner;
    Queue<struct iMessage> * in, * out;
    Semaphore * semH, * semH1, * semO;
    Semaphore * mutex;
    SharedMemory<struct registro> *shm;
    struct registro * registro;

    void activarHidrogeno();
    void activarOxigeno();
    void terminarHidrogeno();
    void terminarOxigeno();
};

#endif	/* REGISTRO_H */

