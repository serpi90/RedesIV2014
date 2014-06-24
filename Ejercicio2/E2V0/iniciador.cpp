#include "includes.h"
#include "Queue.cpp"
#include "SharedMemory.cpp"
#include "Semaphore.h"
#include "Config.h"

using namespace IPC;

int main() {
	std::string owner = "iniciador";

	Queue<ColaPlataforma::message> * pIn, *pOut;
	Queue<ColaExclusion::message> * eIn, *eOut;
	Queue<ColaActivado::message> * act;
	Queue<ColaArmado::message> * arm;
	Queue<ColaDispositivo::message> * disp;
	Queue<ColaSalida::message> * sal;

	Semaphore * mutexPlat;
	SemaphoreArray * semEspera;

	SharedMemory<ColaPlataforma::shared> * shmP;
	ColaPlataforma::shared * plataforma;

	Semaphore * mutexExcl;
	SemaphoreArray * semExcl;

	SharedMemory<ColaExclusion::shared> * shmE;
	ColaExclusion::shared * exclusion;

	pIn = new Queue<ColaPlataforma::message>(path, (long) QueueIdentifier::PLATAFORMA_FROM_INTERFACE, owner);
	pIn->create();
	pOut = new Queue<ColaPlataforma::message>(path, (long) QueueIdentifier::PLATAFORMA_TO_INTERFACE, owner);
	pOut->create();
	eIn = new Queue<ColaExclusion::message>(path, (long) QueueIdentifier::EXCLUSION_FROM_INTERFACE, owner);
	eIn->create();
	eOut = new Queue<ColaExclusion::message>(path, (long) QueueIdentifier::EXCLUSION_TO_INTERFACE, owner);
	eOut->create();
	act = new Queue<ColaActivado::message>(path, (long) QueueIdentifier::ACTIVADO, owner);
	act->create();
	arm = new Queue<ColaArmado::message>(path, (long) QueueIdentifier::ARMADO, owner);
	arm->create();
	disp = new Queue<ColaDispositivo::message>(path, (long) QueueIdentifier::DISPOSITIVOS, owner);
	disp->create();
	sal = new Queue<ColaSalida::message>(path, (long) QueueIdentifier::SALIDA, owner);
	sal->create();

	mutexPlat = new Semaphore(path, (int) (long) SemaphoreIdentifier::MUTEX_PLATAFORMA, owner);
	mutexPlat->create();
	mutexPlat->post();
	semEspera = new SemaphoreArray(path, (int) (long) SemaphoreIdentifier::SEM_ESPERA, ROBOT_AMOUNT, owner);
	semEspera->create();
	shmP = new SharedMemory<ColaPlataforma::shared>(path, (int) (long) SharedMemoryIdentifier::PLATAFORMA, owner);
	shmP->create();
	plataforma = shmP->attach();
	plataforma->amount = 0;
	for (unsigned i = 0; i < ROBOT_AMOUNT; i++) {
		plataforma->robotStatus[i] = ColaPlataforma::RobotStatus::NOT_WAITING;
	}
	for (unsigned i = 0; i < PLATFORM_CAPACITY; i++) {
		plataforma->slot[i].status = ColaPlataforma::SlotStatus::FREE;
	}

	mutexExcl = new Semaphore(path, (int) (long) SemaphoreIdentifier::MUTEX_EXCLUSION, owner);
	mutexExcl->create();
	mutexExcl->post();
	semExcl = new SemaphoreArray(path, (int) (long) SemaphoreIdentifier::SEM_EXCLUSION, ROBOT_AMOUNT, owner);
	semExcl->create();
	shmE = new SharedMemory<ColaExclusion::shared>(path, (int) (long) SharedMemoryIdentifier::EXCLUSION, owner);
	shmE->create();
	exclusion = shmE->attach();

	for (unsigned i = 0; i < ROBOT_AMOUNT; i++) {
		exclusion->armando[i] = ColaExclusion::RobotStatus::IDLE;
		exclusion->sacando[i] = ColaExclusion::RobotStatus::IDLE;
	}
}
