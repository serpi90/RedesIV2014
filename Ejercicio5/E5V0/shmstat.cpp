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
    ss << "+-------------------------------------+" << endl;
    ss << "|             CableCarril             |" << endl;
    ss << "+----+--------+-------------+---------+" << endl;
    ss << "| Nr | Ult Ub.|  Actividad  |  Cargo  |" << endl;
    ss << "+----+--------+-------------+---------+" << endl;
    for (int i = 0; i < CC_AMOUNT; i++)
    {
        ss << "| " << setw(2) << i << " | ";
        ss << setw(6) << Helper::msgToString(copy.cc[i].ubicacion) << " | ";
        ss << setw(20) << Helper::msgToString(copy.cc[i].estado) << " | ";
        ss << setw(3) << copy.cc[i].cantidad << "/" << setw(3)  << CC_SIZE << " |" << endl;
    }
    ss << "+----+---+----+-----------+---------+-----+" << endl;
    ss << "| MAX" << setw(3) << ROOM_SIZE << " | Sala de Arriba | Sala de Abajo |" << endl;
    ss << "+--------+----------------+---------------+" << endl;
    ss << "| Cant   | " << setw(14) << copy.arriba.cantidad << " | " << setw(13) << copy.abajo.cantidad << " |" << endl;
    ss << "| Read   | " << setw(14) << copy.arriba.pRead << " | " << setw(13) << copy.abajo.pRead << " |" << endl;
    ss << "| Write  | " << setw(14) << copy.arriba.pWrite << " | " << setw(13) << copy.abajo.pWrite << " |" << endl;
    for (int i = 0; i < DOOR_AMOUNT; i++)
    {
        ss << "| Pta " << setw(2) << i << " |";
        ss << setw(24) << Helper::msgToString(copy.arriba.estadoPuerta[i]) << " |";
        ss << setw(23) << Helper::msgToString(copy.abajo.estadoPuerta[i]) << " |" << endl;
    }
    ss << "+--------+----------------+---------------+" << endl;
    ss << setfill('0');
    ss << "Arriba: ";
    for (unsigned i = 0; i < copy.arriba.cantidad; i++)
    {
        ss << setw(3) << copy.arriba.personas[i] << " ";
    }
    ss << endl;
    ss << "Abajo:  ";
    for (unsigned i = 0; i < copy.abajo.cantidad; i++)
    {
        ss << setw(3) << copy.abajo.personas[i] << " ";
    }
    ss << endl;
    Helper::output(stdout, ss);
    shm->dettach();
}
