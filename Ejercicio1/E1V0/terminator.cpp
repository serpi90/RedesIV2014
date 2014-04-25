/* 
 * File:   terminator.cpp
 * Author: Julian Maestri
 *
 * Created on April 6, 2014, 3:00 PM
 */

#include <cstdlib>
#include "includes.h"
#include "Queue.h"
#include "SharedMemory.h"
#include "Semaphore.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv)
{
    Queue * queue;
    SemaphoreArray * sem;
    SharedMemory * shm;
    queue = new Queue(PATH, Q_PLAT_TO_INTERFACE, "terminator", false);
    queue->get();
    queue->remove();
    queue = new Queue(PATH, Q_PLAT_FROM_INTERFACE, "terminator", false);
    queue->get();
    queue->remove();
    sem = new SemaphoreArray(PATH, SEM_MUTEX, 1, "terminator", false);
    sem->get();
    sem->remove();
    sem = new SemaphoreArray(PATH, SEM_SEMA, ROBOT_AMOUNT, "terminator", false);
    sem->get();
    sem->remove();
    sem = new SemaphoreArray(PATH, SEM_SEMB, ROBOT_AMOUNT, "terminator", false);
    sem->get();
    sem->remove();
    sem = new SemaphoreArray(PATH, SEM_SEME, ROBOT_AMOUNT, "terminator", false);
    sem->get();
    sem->remove();
    shm = new SharedMemory(PATH, SHM_PLAT, sizeof (struct plate), "terminator", false);
    shm->get();
    shm->remove();
    return 0;
}

