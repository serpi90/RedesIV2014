#include "includes.h"
#include "Queue.cpp"
#include "Semaphore.h"

int main()
{
    Queue<struct msgAlmacen> * qa;

    qa = new Queue<struct msgAlmacen>(PATH, Q_FROM_INTERFACE_TO_CTL, "iniciador");
    qa->create();
    qa = new Queue<struct msgAlmacen>(PATH, Q_FROM_CTL_TO_NET, "iniciador");
    qa->create();
    qa = new Queue<struct msgAlmacen>(PATH, Q_FROM_NET_TO_INTERFACE, "iniciador");
    qa->create();

    Semaphore mutex(PATH, SEM_MUTEX_IDM, "iniciador");
    mutex.create();
    mutex.post();
}
