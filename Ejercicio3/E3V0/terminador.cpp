#include "includes.h"
#include "Queue.cpp"

int main()
{
    Queue<struct msgAlmacen> * qa;

    qa = new Queue<struct msgAlmacen>(PATH, Q_ALMACEN, "terminador", false);
    qa->get();
    qa->remove();
}
