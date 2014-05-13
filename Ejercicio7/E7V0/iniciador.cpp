#include "includes.h"
#include "Semaphore.h"
#include "Queue.cpp"

int main()
{
    // Create the queues
    Queue<struct iMessage> * q;

    q = new Queue<struct iMessage>(PATH, Q_FROM_INTERFACE, "iniciador");
    q->create();
    q = new Queue<struct iMessage>(PATH, Q_TO_INTERFACE, "iniciador");
    q->create();
    q = new Queue<struct iMessage>(PATH, Q_TO_IREPOSITOR, "iniciador");
    q->create();
    q = new Queue<struct iMessage>(PATH, Q_FROM_IREPOSITOR, "iniciador");
    q->create();

    // Create and initialize the semaphores
    Semaphore * s;

    s = new Semaphore(PATH, SEM_PINTAR, "iniciador");
    s->create();
    s = new Semaphore(PATH, SEM_PINTAR2, "iniciador");
    s->create();
    s = new Semaphore(PATH, SEM_SECAR, "iniciador");
    s->create();
    s = new Semaphore(PATH, SEM_SECAR2, "iniciador");
    s->create();
    s = new Semaphore(PATH, SEM_MOTOR, "iniciador");
    s->create();
    s = new Semaphore(PATH, SEM_MOTOR2, "iniciador");
    s->create();
    s = new Semaphore(PATH, SEM_INTERIOR, "iniciador");
    s->create();
    s = new Semaphore(PATH, SEM_INTERIOR2, "iniciador");
    s->create();
    s = new Semaphore(PATH, SEM_EXTERIOR, "iniciador");
    s->create();
    s = new Semaphore(PATH, SEM_EXTERIOR2, "iniciador");
    s->create();

    pid_t pid = fork();
    if (pid == 0)
    {
        execlp("./componente", "componente", NULL);
        perror("Componente - execlp: ");
        exit(EXIT_FAILURE);
    } else if (pid < 0)
    {
        perror("Componente - fork: ");
    }

}
