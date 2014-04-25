/* 
 * File:   shmstat.cpp
 * Author: Julian Maestri
 *
 * Created on April 6, 2014, 1:34 PM
 */

#include "includes.h"
#include "Semaphore.h"
#include "SharedMemory.h"
#include <string>
#include <sstream>

std::string msgRobot(enum robotStatus s)
{
    switch (s)
    {
    case OFF:
        return "OFF";
    case IDLE:
        return "IDLE";
    case WORKING:
        return "WORKING";
    default:
        return "?";
    }
}
std::string msgDisp(enum dispStatus s)
{
    switch (s)
    {
    case ACTIVE:
        return "ACTIVE";
    case INACTIVE:
        return "INACTIVE";
    default:
        return "?";
    }
}
std::string msgSlot(enum slotStatus s)
{
    switch (s)
    {
    case OCCUPIED:
        return "OCCUPIED";
    case RESERVED:
        return "RESERVED";
    case EMPTY:
        return "EMPTY";
    default:
        return "?";
    }
}
std::string msgWait(enum robotWaitingStatus s)
{
    switch (s)
    {
    case WAITING:
        return "WAITING";
    case NOT_WAITING:
        return "NOT WAITING";
    default:
        return "?";
    }
}
int main( ) {
    SharedMemory * shm = new SharedMemory(PATH, SHM_PLAT, sizeof (struct plate), "shmstat");
    shm->get();
    struct plate * plate = (struct plate*) shm->attach();
    std::stringstream status;
    Semaphore * mutex = new Semaphore(PATH, SEM_MUTEX, "shmstat");
    mutex->get();
    mutex->wait();
    status << "RobotsA: " << plate->activeRobotsA << std::endl;
    status << "RobotsB: " << plate->activeRobotsB << std::endl;
    status << "Dispositivos: " << plate->storedAmount << std::endl;
    for (int i = 0; i < ROBOT_AMOUNT; i++)
    {
        status << "A" << i << ": " << msgRobot( plate->statusA[i] ) << ", " << msgWait( plate->statusE[i] ) << std::endl;
        status << "B" << i << ": " << msgRobot( plate->statusB[i] ) << std::endl;
    }
    for (int i = 0; i < PLAT_SIZE; i++)
    {
	status << i << ": " << msgSlot(plate->slots[i].state) << ", " << msgDisp(plate->slots[i].dispositive.status) << std::endl;
    }
    mutex->post();
    write(fileno(stdout),status.str().c_str(),status.str().size());
    shm->dettach();
}

