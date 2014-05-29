#include "includes.h"
#include "Queue.cpp"

int main()
{
    Queue<struct iMessage> * q;

    q = new Queue<struct iMessage>(PATH, Q_FROM_INTERFACE_TO_NET, "iniciador");
    q->create();
    q = new Queue<struct iMessage>(PATH, Q_FROM_NET_TO_COMPONENT, "iniciador");
    q->create();
    q = new Queue<struct iMessage>(PATH, Q_FROM_COMPONENT_TO_NET, "iniciador");
    q->create();
    q = new Queue<struct iMessage>(PATH, Q_FROM_NET_TO_INTERFACE, "iniciador");
    q->create();

    Queue<struct msgAlmacen> * qa;

    qa = new Queue<struct msgAlmacen>(PATH, Q_ALMACEN, "iniciador");
    qa->create();

    pid_t pid = fork();
    if (pid == 0)
    {
        execlp("./almacen", "almacen", NULL);
        perror("almacen - execlp: ");
        exit(EXIT_FAILURE);
    } else if (pid < 0)
    {
        perror("almacen - fork: ");
    }

}
