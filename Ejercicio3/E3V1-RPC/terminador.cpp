#include "includes.h"
#include "Queue.cpp"
#include "Semaphore.h"

int main()
{
    Queue<struct iMessage> * q;

    q = new Queue<struct iMessage>(PATH, Q_FROM_INTERFACE_TO_CTL, "terminador", false);
    q->get();
    q->remove();
    q = new Queue<struct iMessage>(PATH, Q_FROM_CTL_TO_NET, "terminador", false);
    q->get();
    q->remove();
    q = new Queue<struct iMessage>(PATH, Q_FROM_NET_TO_INTERFACE, "terminador", false);
    q->get();
    q->remove();

    system("pkill net-receiver");
    system("pkill net-constroller");
}
