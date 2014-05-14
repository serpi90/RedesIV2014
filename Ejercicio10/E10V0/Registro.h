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
    Semaphore * cotiz, * esperaLectores, * esperaEscritores;
    Semaphore * mutex;
    SharedMemory<struct registro> * shm;
    struct registro * registro;

    double leer();
    void escribir(double);
};

#endif	/* REGISTRO_H */

