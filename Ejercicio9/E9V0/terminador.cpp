#include "includes.h"
#include "Queue.cpp"
#include "SharedMemory.cpp"
#include "Semaphore.h"

int main()
{
    Queue<struct iMessage> * q;

    q = new Queue<struct iMessage>(PATH, Q_FROM_INTERFACE, "terminador", false);
    q->get();
    q->remove();
    q = new Queue<struct iMessage>(PATH, Q_TO_INTERFACE, "terminador", false);
    q->get();
    q->remove();

    Semaphore * s;
    
    s = new Semaphore(PATH, SEM_MUTEX, "terminador", false);
    s->get();
    s->remove();
    s = new Semaphore(PATH, SEM_H, "terminador", false);
    s->get();
    s->remove();
    s = new Semaphore(PATH, SEM_H1, "terminador", false);
    s->get();
    s->remove();
    s = new Semaphore(PATH, SEM_O, "terminador", false);
    s->get();
    s->remove();


    SharedMemory<struct registro> * shm;
    shm = new SharedMemory<struct registro>(PATH, SHM_REGISTRO, "terminador", false);
    shm->get();
    shm->remove();
}