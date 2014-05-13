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
    Semaphore * sems;
    Semaphore * mutex;
    SharedMemory<struct registro> *shm;
    struct registro * registro;

    void termine(void);
};

#endif	/* REGISTRO_H */

