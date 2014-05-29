#include "includes.h"
#include "Queue.cpp"

int main()
{
    Queue<struct iMessage> * q;

    q = new Queue<struct iMessage>(PATH, Q_FROM_INTERFACE, "terminador", false);
    q->get();
    q->remove();
    q = new Queue<struct iMessage>(PATH, Q_TO_INTERFACE, "terminador", false);
    q->get();
    q->remove();

    Queue<struct msgAlmacen> * qa;

    qa = new Queue<struct msgAlmacen>(PATH, Q_ALMACEN, "terminador", false);
    qa->get();
    qa->remove();
}
