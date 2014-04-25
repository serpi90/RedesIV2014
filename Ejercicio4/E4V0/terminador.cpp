#include "includes.h"
#include "Semaphore.h"
#include "SharedMemory.cpp"
#include "Queue.cpp"

int main()
{
    SemaphoreArray * s;
    Queue<struct iMessage> * qi;
    Queue<struct personMessage> * qp;
    SharedMemory<struct cableCarril> * shm;

    qi = new Queue<struct iMessage>(PATH, Q_SALA_FROM_INTERFACE, "terminador", false);
    qi->get();
    qi->remove();
    qi = new Queue<struct iMessage>(PATH, Q_SALA_TO_INTERFACE, "terminador", false);
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
    shm = new SharedMemory<struct cableCarril>(PATH, SHM_CC, "terminador", false);
    shm->get();
    shm->remove();
    s = new SemaphoreArray(PATH, SEM_MUTEX, 1, "terminador", false);
    s->get();
    s->remove();
    s = new SemaphoreArray(PATH, SEM_FULL, 2, "terminador", false);
    s->get();
    s->remove();
    s = new SemaphoreArray(PATH, SEM_EMPTY, 1, "terminador", false);
    s->get();
    s->remove();
}