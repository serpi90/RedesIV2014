#include "includes.h"
#include "Queue.cpp"
#include "SharedMemory.cpp"
#include "Semaphore.h"

int main()
{
    Queue<struct iMessage> * q;

    q = new Queue<struct iMessage>(PATH, Q_FROM_INTERFACE, "iniciador");
    q->create();
    q = new Queue<struct iMessage>(PATH, Q_TO_INTERFACE, "iniciador");
    q->create();

    Semaphore * s;

    s = new Semaphore(PATH, SEM_MUTEX, "iniciador");
    s->create();
    s->post();
    s = new Semaphore(PATH, SEM_H, "iniciador");
    s->create();
    s->post();
    s = new Semaphore(PATH, SEM_H1, "iniciador");
    s->create();
    s = new Semaphore(PATH, SEM_O, "iniciador");
    s->create();

    SharedMemory<struct registro> * shm;
    struct registro * reg;

    shm = new SharedMemory<struct registro>(PATH, SHM_REGISTRO, "iniciador");
    shm->create();
    reg = shm->attach();
    reg->activos = 0;
    reg->hidrogenos = 0;

    pid_t pid = fork();
    if (pid == 0)
    {
        execlp("./registro", "registro", NULL);
        perror("registro - execlp: ");
        exit(EXIT_FAILURE);
    } else if (pid < 0)
    {
        perror("registro - fork: ");
    }

}
