#include "includes.h"
#include "SharedMemory.cpp"
#include "Semaphore.h"
#include <iostream>
#include <iomanip>

using namespace std;

int main()
{
    Semaphore * mutex;

    mutex = new Semaphore(PATH, SEM_MUTEX, "terminador", false);
    mutex->get();

    SharedMemory<struct registro> * shm;
    shm = new SharedMemory<struct registro>(PATH, SHM_REGISTRO, "terminador", false);
    shm->get();
    struct registro * reg = shm->attach();
    struct registro copy;
    mutex->wait();
    copy = *reg;
    mutex->post();
    cout << "+------------+-----------+------------+-------+" << std::endl;
    cout << "|            | Esperando | Trabajando | Otros |" << std::endl;
    cout << "+------------+-----------+------------+-------+" << std::endl;
    cout << "| Escritores | " << setw(9) << copy.escritoresEsperando << " | " << setw(10) << copy.escritorEscribiendo << " | " << setw(5) << copy.cotizacion << " | " << std::endl;
    cout << "+------------+-----------+------------+-------+" << std::endl;
    cout << "| Lectores   | " << setw(9) << copy.lectoresEsperando << " | " << setw(10) << copy.lectoresLeyendo << " | " << setw(5) << copy.lectores << " | " << std::endl;
    cout << "+------------+-----------+------------+-------+" << std::endl;
}
