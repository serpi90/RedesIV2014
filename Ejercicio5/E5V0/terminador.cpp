#include "includes.h"
#include "Semaphore.h"
#include "SharedMemory.cpp"
#include "Queue.cpp"

int main()
{
    SemaphoreArray * s;
    Queue<struct iMessage> * qi;
    Queue<struct personMessage> * qp;
    SharedMemory<struct registro> * shm;

    qi = new Queue<struct iMessage>(PATH, Q_PUERTA_FROM_INTERFACE, "terminador", false);
    qi->get();
    qi->remove();
    qi = new Queue<struct iMessage>(PATH, Q_PUERTA_TO_INTERFACE, "terminador", false);
    qi->get();
    qi->remove();
    qi = new Queue<struct iMessage>(PATH, Q_CC_TO_INTERFACE, "terminador", false);
    qi->get();
    qi->remove();
    qi = new Queue<struct iMessage>(PATH, Q_CC_FROM_INTERFACE, "terminador", false);
    qi->get();
    qi->remove();
    qp = new Queue<struct personMessage>(PATH, Q_PERSONA, "terminador", false);
    qp->get();
    qp->remove();
    qp = new Queue<struct personMessage>(PATH, Q_SALA_ABAJO, "terminador", false);
    qp->get();
    qp->remove();
    qp = new Queue<struct personMessage>(PATH, Q_SALA_ARRIBA, "terminador", false);
    qp->get();
    qp->remove();
    qp = new Queue<struct personMessage>(PATH, Q_CC, "terminador", false);
    qp->get();
    qp->remove();
    shm = new SharedMemory<struct registro>(PATH, SHM_CC_SALAS, "terminador", false);
    shm->get();
    shm->remove();
    s = new SemaphoreArray(PATH, SEM_MUTEX, 1, "terminador", false);
    s->get();
    s->remove();
    s = new SemaphoreArray(PATH, SEM_ABAJO, 1, "terminador", false);
    s->get();
    s->remove();
    s = new SemaphoreArray(PATH, SEM_ARRIBA, 1, "terminador", false);
    s->get();
    s->remove();
    s = new SemaphoreArray(PATH, SEM_FULL_BOTTOM, DOOR_AMOUNT, "terminador", false);
    s->get();
    s->remove();
    s = new SemaphoreArray(PATH, SEM_FULL_TOP, DOOR_AMOUNT, "terminador", false);
    s->get();
    s->remove();
    s = new SemaphoreArray(PATH, SEM_EMPTY, CC_AMOUNT, "terminador", false);
    s->get();
    s->remove();
}