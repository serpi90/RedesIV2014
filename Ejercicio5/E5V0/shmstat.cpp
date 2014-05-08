#include <sstream>
#include <iomanip>
#include <string>
#include "includes.h"
#include "Semaphore.h"
#include "SharedMemory.cpp"
#include "Helper.h"

using namespace std;

int main()
{
    Semaphore * mutex;
    SharedMemory<struct registro> * shm;
    struct registro * registro, copy;
    stringstream ss;

    shm = new SharedMemory<struct registro>(PATH, SHM_CC_SALAS, "shmstat");
    shm->get();
    registro = shm->attach();

    mutex = new Semaphore(PATH, SEM_MUTEX, "shmstat");
    mutex->get();
    mutex->wait();
    copy = *registro;
    mutex->post();
    for (int i = 0; i < CC_AMOUNT; i++)
    {
        ss << "CC: " << setw(10) << Helper::msgToString(copy.cc[i].estado) << " " << setw(7) << Helper::msgToString(copy.cc[i].ubicacion) << endl;
    }

    ss << "ARRIBA: " << "r:" << setw(3) << copy.arriba.pRead << " w:" << setw(3) << copy.arriba.pWrite << "(" << setw(3) << copy.arriba.cantidad << ") " << endl;
    ss << " en sala: ";
    for (unsigned i = 0; i < copy.arriba.cantidad; i++)
    {
        ss << setw(3) << copy.arriba.personas[i] << " ";
    }
    ss << endl;
    ss << "puertas: ";
    for (int i = 0; i < ROOM_AMOUNT; i++)
    {
        ss << setw(6) << Helper::msgToString(copy.arriba.estadoPuerta[i]) << " ";
    }
    ss << endl;

    ss << "ABAJO:  " << "r:" << setw(3) << copy.abajo.pRead << " w:" << setw(3) << copy.abajo.pWrite << "(" << setw(3) << copy.abajo.cantidad << ") " << endl;
    ss << " en sala: ";
    for (unsigned i = 0; i < copy.abajo.cantidad; i++)
    {
        ss << setw(3) << copy.abajo.personas[i] << " ";
    }
    ss << endl;
    ss << "puertas: ";
    for (int i = 0; i < ROOM_AMOUNT; i++)
    {
        ss << setw(6) << Helper::msgToString(copy.abajo.estadoPuerta[i]) << " ";
    }
    ss << endl;
    Helper::output(stdout, ss);
    shm->dettach();
}
