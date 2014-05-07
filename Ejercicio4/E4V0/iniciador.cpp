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
    struct registro * registro;

    qi = new Queue<struct iMessage>(PATH, Q_SALA_TO_INTERFACE, "iniciador");
    qi->create();
    qi = new Queue<struct iMessage>(PATH, Q_SALA_FROM_INTERFACE, "iniciador");
    qi->create();
    qi = new Queue<struct iMessage>(PATH, Q_CC_TO_INTERFACE, "iniciador");
    qi->create();
    qi = new Queue<struct iMessage>(PATH, Q_CC_FROM_INTERFACE, "iniciador");
    qi->create();
    qp = new Queue<struct personMessage>(PATH, Q_PERSONA, "iniciador");
    qp->create();
    qp = new Queue<struct personMessage>(PATH, Q_SALA_ABAJO, "iniciador");
    qp->create();
    qp = new Queue<struct personMessage>(PATH, Q_SALA_ARRIBA, "iniciador");
    qp->create();
    qp = new Queue<struct personMessage>(PATH, Q_CC, "iniciador");
    qp->create();
    shm = new SharedMemory<struct registro>(PATH, SHM_CC, "iniciador");
    shm->create();
    registro = shm->attach();

    registro->cc.estado = WORKING;
    registro->cc.ubicacion = BOTTOM;

    registro->abajo.estado = WORKING;
    registro->abajo.cantidad = 0;
    registro->abajo.pRead = 0;
    registro->abajo.pWrite = 0;
    for (int i = 0; i < ROOM_SIZE; i++)
    {
        registro->abajo.personas[i] = 0;
    }

    registro->arriba.estado = WORKING;
    registro->arriba.cantidad = 0;
    registro->arriba.pRead = 0;
    registro->arriba.pWrite = 0;
    for (int i = 0; i < ROOM_SIZE; i++)
    {
        registro->arriba.personas[i] = 0;
    }

    s = new SemaphoreArray(PATH, SEM_MUTEX, 1, "iniciador");
    s->create();
    s->post(0);
    s = new SemaphoreArray(PATH, SEM_FULL, 2, "iniciador");
    s->create();
    s = new SemaphoreArray(PATH, SEM_EMPTY, 1, "iniciador");
    s->create();


    std::stringstream ss;
    pid_t pid;
    pid = fork();
    if (pid == 0)
    {
        execlp("./comp-sala", "comp-sala", NULL);
        perror("Componente Sala - execlp: ");
        exit(EXIT_FAILURE);
    } else if (pid < 0)
    {
        perror("Componente Sala - fork: ");
    }
    pid = fork();
    if (pid == 0)
    {
        execlp("./comp-cablecarril", "comp-cablecarril", NULL);
        perror("Componente CableCarril - execlp: ");
        exit(EXIT_FAILURE);
    } else if (pid < 0)
    {
        perror("Componente CableCarril - fork: ");
    }

    pid = fork();
    if (pid == 0)
    {
        execlp("./sala", "sala", "-abajo", NULL);
        perror("Sala Abajo - execlp: ");
        exit(EXIT_FAILURE);
    } else if (pid < 0)
    {
        perror("Sala Abajo - fork: ");
    }
    pid = fork();
    if (pid == 0)
    {
        execlp("./sala", "sala", "-arriba", NULL);
        perror("Sala Arriba - execlp: ");
        exit(EXIT_FAILURE);
    } else if (pid < 0)
    {
        perror("Sala Arriba - fork: ");
    }
    pid = fork();
    if (pid == 0)
    {
        execlp("./cablecarril", "cablecarril", NULL);
        perror("CableCarril - execlp: ");
        exit(EXIT_FAILURE);
    } else if (pid < 0)
    {
        perror("CableCarril - fork: ");
    }
}