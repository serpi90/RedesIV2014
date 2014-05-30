#include "includes.h"
#include "Queue.cpp"

int main()
{
    Queue<struct msgAlmacen> * qa;

    qa = new Queue<struct msgAlmacen>(PATH, Q_FROM_INTERFACE_TO_NET, "iniciador");
    qa->create();
    qa = new Queue<struct msgAlmacen>(PATH, Q_FROM_NET_TO_INTERFACE, "iniciador");
    qa->create();
}
