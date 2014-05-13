#include "includes.h"
#include "Semaphore.h"
#include "Queue.cpp"

int main()
{
    // Create the queues
    Queue<struct iMessage> * q;

    q = new Queue<struct iMessage>(PATH, Q_FROM_INTERFACE, "terminador", false);
    q->get();
    q->remove();
    q = new Queue<struct iMessage>(PATH, Q_TO_INTERFACE, "terminador", false);
    q->get();
    q->remove();
    q = new Queue<struct iMessage>(PATH, Q_TO_IREPOSITOR, "terminador", false);
    q->get();
    q->remove();
    q = new Queue<struct iMessage>(PATH, Q_FROM_IREPOSITOR, "terminador", false);
    q->get();
    q->remove();

    // Create and initialize the semaphores
    Semaphore * s;

    s = new Semaphore(PATH, SEM_PINTAR, "terminador", false);
    s->get();
    s->remove();
    s = new Semaphore(PATH, SEM_PINTAR2, "terminador", false);
    s->get();
    s->remove();
    s = new Semaphore(PATH, SEM_SECAR, "terminador", false);
    s->get();
    s->remove();
    s = new Semaphore(PATH, SEM_SECAR2, "terminador", false);
    s->get();
    s->remove();
    s = new Semaphore(PATH, SEM_MOTOR, "terminador", false);
    s->get();
    s->remove();
    s = new Semaphore(PATH, SEM_MOTOR2, "terminador", false);
    s->get();
    s->remove();
    s = new Semaphore(PATH, SEM_INTERIOR, "terminador", false);
    s->get();
    s->remove();
    s = new Semaphore(PATH, SEM_INTERIOR2, "terminador", false);
    s->get();
    s->remove();
    s = new Semaphore(PATH, SEM_EXTERIOR, "terminador", false);
    s->get();
    s->remove();
    s = new Semaphore(PATH, SEM_EXTERIOR2, "terminador", false);
    s->get();
    s->remove();
}