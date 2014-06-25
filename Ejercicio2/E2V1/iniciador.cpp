#include <string>

#include "includes.h"
#include "Queue.cpp"
#include "Semaphore.h"
#include "SemaphoreArray.h"
#include "SharedMemory.cpp"

using namespace IPC;

int main() {
	std::string owner = "iniciador";

	Queue<ColaPlataforma::message> * plat;
	Queue<ColaExclusion::message> * excl;
	Queue<ColaActivado::message> * act;
	Queue<ColaArmado::message> * arm;
	Queue<ColaDispositivo::message> * disp;
	Queue<ColaSalida::message> * sal;
	Queue<Net::iMessage> * net;

	Semaphore * mutexPlat;
	SemaphoreArray * semEspera;

	SharedMemory<ColaPlataforma::shared> * shmP;
	ColaPlataforma::shared * plataforma;

	Semaphore * mutexExcl;
	SemaphoreArray * semExcl;

	SharedMemory<ColaExclusion::shared> * shmE;
	ColaExclusion::shared * exclusion;

	Semaphore * mutexIdm = new Semaphore(IPC::path, (long) IPC::SemaphoreIdentifier::MUTEX_ID_MANAGER, owner);
	mutexIdm->create();

	net = new Queue<Net::iMessage>(IPC::path, (long) IPC::QueueIdentifier::FROM_CTL_TO_NET, owner);
	net->create();

	arm = new Queue<ColaArmado::message>(IPC::path, (long) IPC::QueueIdentifier::ARMADO_FROM_DISP_TO_CTL, owner);
	arm->create();
	arm = new Queue<ColaArmado::message>(IPC::path, (long) IPC::QueueIdentifier::ARMADO_FROM_CTL_TO_INTERFACE, owner);
	arm->create();

	act = new Queue<ColaActivado::message>(IPC::path, (long) IPC::QueueIdentifier::ACTIVADO_FROM_DISP_TO_CTL, owner);
	act->create();
	act = new Queue<ColaActivado::message>(IPC::path, (long) IPC::QueueIdentifier::ACTIVADO_FROM_CTL_TO_INTERFACE, owner);
	act->create();

	sal = new Queue<ColaSalida::message>(IPC::path, (long) IPC::QueueIdentifier::SALIDA_FROM_INTERFACE_TO_CTL, owner);
	sal->create();
	sal = new Queue<ColaSalida::message>(IPC::path, (long) IPC::QueueIdentifier::SALIDA_FROM_CTL_TO_INTERFACE, owner);
	sal->create();

	disp = new Queue<ColaDispositivo::message>(IPC::path, (long) IPC::QueueIdentifier::DISPOSITIVOS_FROM_PLATAFORMA_TO_CTL, owner);
	disp->create();
	disp = new Queue<ColaDispositivo::message>(IPC::path, (long) IPC::QueueIdentifier::DISPOSITIVOS_FROM_CTL_TO_DISP, owner);
	disp->create();

	plat = new Queue<ColaPlataforma::message>(IPC::path, (long) IPC::QueueIdentifier::FROM_INTERFACE_TO_PLATAFORMA, owner);
	plat->create();
	plat = new Queue<ColaPlataforma::message>(IPC::path, (long) IPC::QueueIdentifier::FROM_PLATAFORMA_TO_INTERFACE, owner);
	plat->create();

	excl = new Queue<ColaExclusion::message>(IPC::path, (long) IPC::QueueIdentifier::FROM_INTERFACE_TO_EXCLUSION, owner);
	excl->create();
	excl = new Queue<ColaExclusion::message>(IPC::path, (long) IPC::QueueIdentifier::FROM_EXCLUSION_TO_INTERFACE, owner);
	excl->create();

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