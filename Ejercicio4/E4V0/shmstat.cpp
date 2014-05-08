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

    shm = new SharedMemory<struct registro>(PATH, SHM_CC, "shmstat");
    shm->get();
    registro = shm->attach();

    mutex = new Semaphore(PATH, SEM_MUTEX, "shmstat");
    mutex->get();
    mutex->wait();
    copy = *registro;
    mutex->post();
    ss << "CC: " << setw(10) << Helper::msgToString(copy.cc.estado) << " " << setw(7) << Helper::msgToString(copy.cc.ubicacion) << endl;
    ss << "AR: " << setw(10) << Helper::msgToString(copy.arriba.estado) << "   (" << setw(3) << copy.arriba.cantidad << ") " << "r:" << setw(3) << copy.arriba.pRead << " w:" << setw(3) << copy.arriba.pWrite;
    ss << " en sala: ";
    for (unsigned i = 0; i < copy.arriba.cantidad; i++)
    {
        ss << setw(3) << copy.arriba.personas[i] << " ";
    }
    ss << endl;
    ss << "AB: " << setw(10) << Helper::msgToString(copy.abajo.estado) << "   (" << setw(3) << copy.abajo.cantidad << ") " << "r:" << setw(3) << copy.abajo.pRead << " w:" << setw(3) << copy.abajo.pWrite;
    ss << " en sala: ";
    for (unsigned i = 0; i < copy.abajo.cantidad; i++)
    {
        ss << setw(3) << copy.abajo.personas[i] << " ";
    }
    ss << endl;
    Helper::output(stdout, ss);
    shm->dettach();
}
