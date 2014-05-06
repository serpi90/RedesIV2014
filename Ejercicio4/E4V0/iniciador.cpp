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
    struct cableCarril * cableCarril;

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
    shm = new SharedMemory<struct cableCarril>(PATH, SHM_CC, "iniciador");
    shm->create();
    cableCarril = shm->attach();
    cableCarril->cantAbajo = 0;
    cableCarril->estadoArriba = WAITING;
    cableCarril->estadoAbajo = WAITING;
    cableCarril->estadoCablecarril = WAITING;
    cableCarril->ubicacionCablecarril = BOTTOM;
    for (int i = 0; i < ROOM_SIZE; i++)
    {
        cableCarril->personasArriba[i] = 0;
    }
    cableCarril->cantArriba = 0;
    cableCarril->pReadArriba = 0;
    cableCarril->pWriteArriba = 0;
    for (int i = 0; i < ROOM_SIZE; i++)
    {
        cableCarril->personasAbajo[i] = 0;
    }
    cableCarril->cantAbajo = 0;
    cableCarril->pReadAbajo = 0;
    cableCarril->pWriteAbajo = 0;

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