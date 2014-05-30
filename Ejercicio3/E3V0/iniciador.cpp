#include "includes.h"
#include "Queue.cpp"

int main()
{
    Queue<struct msgAlmacen> * qa;

    qa = new Queue<struct msgAlmacen>(PATH, Q_ALMACEN, "iniciador");
    qa->create();
}
