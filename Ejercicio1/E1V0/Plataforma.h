/* 
 * File:   Plataforma.h
 * Author: lubuntu
 *
 * Created on April 6, 2014, 2:20 PM
 */

#ifndef PLATAFORMA_H
#define	PLATAFORMA_H
#include "includes.h"
#include "Queue.h"
#include "Semaphore.h"
#include "SharedMemory.h"

class Plataforma {
public:
    Plataforma(int number, char rtype);
    void init();
private:
    int id;
    std::string owner;
    int reserve(int robot);
    void put(int robot, int location, long id);
    bool take(int robot);
    bool haveDispOrRobots();
    bool shutdown(int robot, char rtype);
    std::string msgToStr(enum iPlatMessage function);
    Queue * fromInterface;
    Queue * toInterface;
    Semaphore * mutex;
    SemaphoreArray * semsA;
    SemaphoreArray * semsB;
    SemaphoreArray * semsE;
    SharedMemory * shm;
    struct plate * plate;
};

#endif	/* PLATAFORMA_H */

