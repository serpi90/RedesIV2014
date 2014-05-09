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

    qi = new Queue<struct iMessage>(PATH, Q_PUERTA_TO_INTERFACE, "iniciador");
    qi->create();
    qi = new Queue<struct iMessage>(PATH, Q_PUERTA_FROM_INTERFACE, "iniciador");
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
    shm = new SharedMemory<struct registro>(PATH, SHM_CC_SALAS, "iniciador");
    shm->create();
    registro = shm->attach();

    for (int i = 0; i < CC_AMOUNT; i++)
    {
        registro->cc[i].estado = WORKING;
        registro->cc[i].ubicacion = ABAJO;
    }

    registro->abajo.cantidad = 0;
    registro->abajo.pRead = 0;
    registro->abajo.pWrite = 0;
    for (int i = 0; i < DOOR_AMOUNT; i++)
    {
        registro->abajo.estadoPuerta[i] = WORKING;
    }
    for (int i = 0; i < ROOM_SIZE; i++)
    {
        registro->abajo.personas[i] = 0;
    }

    registro->arriba.cantidad = 0;
    registro->arriba.pRead = 0;
    registro->arriba.pWrite = 0;
    for (int i = 0; i < DOOR_AMOUNT; i++)
    {
        registro->arriba.estadoPuerta[i] = WORKING;
    }
    for (int i = 0; i < ROOM_SIZE; i++)
    {
        registro->arriba.personas[i] = 0;
    }

    s = new SemaphoreArray(PATH, SEM_MUTEX, 1, "iniciador");
    s->create();
    s->post(0);
    s = new SemaphoreArray(PATH, SEM_ABAJO, 1, "iniciador");
    s->create();
    s->post(0);
    s = new SemaphoreArray(PATH, SEM_ARRIBA, 1, "iniciador");
    s->create();
    s->post(0);
    s = new SemaphoreArray(PATH, SEM_FULL_BOTTOM, DOOR_AMOUNT, "iniciador");
    s->create();
    s = new SemaphoreArray(PATH, SEM_FULL_TOP, DOOR_AMOUNT, "iniciador");
    s->create();
    s = new SemaphoreArray(PATH, SEM_EMPTY, CC_AMOUNT, "iniciador");
    s->create();


    std::stringstream ss;
    pid_t pid;
    pid = fork();
    if (pid == 0)
    {
        execlp("./comp-puerta", "comp-puerta", NULL);
        perror("Componente Puerta - execlp: ");
        exit(EXIT_FAILURE);
    } else if (pid < 0)
    {
        perror("Componente Puerta - fork: ");
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

    for (int i = 0; i < DOOR_AMOUNT; i++)
    {
        std::stringstream ss;
        ss << i;
        pid = fork();
        if (pid == 0)
        {
            execlp("./puerta", "puerta", "-abajo", ss.str().c_str(), NULL);
            perror("Puerta Abajo - execlp: ");
            exit(EXIT_FAILURE);
        } else if (pid < 0)
        {
            perror("Puerta Abajo - fork: ");
        }
        pid = fork();
        if (pid == 0)
        {
            execlp("./puerta", "puerta", "-arriba", ss.str().c_str(), NULL);
            perror("Puerta Arriba - execlp: ");
            exit(EXIT_FAILURE);
        } else if (pid < 0)
        {
            perror("Puerta Arriba - fork: ");
        }
    }
    for (int i = 0; i < CC_AMOUNT; i++)
    {
        std::stringstream ss;
        ss << i;
        pid = fork();
        if (pid == 0)
        {
            execlp("./cablecarril", "cablecarril", ss.str().c_str(), NULL);
            perror("CableCarril - execlp: ");
            exit(EXIT_FAILURE);
        } else if (pid < 0)
        {
            perror("CableCarril - fork: ");
        }
    }
}