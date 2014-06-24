#include "includes.h"
#include "Queue.cpp"
#include "SharedMemory.cpp"
#include "Semaphore.h"
#include "Config.h"

using namespace IPC;

int main() {
	std::string owner = "terminador";

	Queue<ColaPlataforma::message> * pIn, *pOut;
	Queue<ColaExclusion::message> * eIn, *eOut;
	Queue<ColaActivado::message> * act;
	Queue<ColaArmado::message> * arm;
	Queue<ColaDispositivo::message> * disp;
	Queue<ColaSalida::message> * sal;

	Semaphore * mutexPlat;
	SemaphoreArray * semEspera;

	SharedMemory<ColaPlataforma::shared> * shmP;

	Semaphore * mutexExcl;
	SemaphoreArray * semExcl;

	SharedMemory<ColaExclusion::shared> * shmE;

	pIn = new Queue<ColaPlataforma::message>(path, (long) QueueIdentifier::PLATAFORMA_FROM_INTERFACE, owner, false);
	pIn->get();
	pIn->remove();
	pOut = new Queue<ColaPlataforma::message>(path, (long) QueueIdentifier::PLATAFORMA_TO_INTERFACE, owner, false);
	pOut->get();
	pOut->remove();
	eIn = new Queue<ColaExclusion::message>(path, (long) QueueIdentifier::EXCLUSION_FROM_INTERFACE, owner, false);
	eIn->get();
	eIn->remove();
	eOut = new Queue<ColaExclusion::message>(path, (long) QueueIdentifier::EXCLUSION_TO_INTERFACE, owner, false);
	eOut->get();
	eOut->remove();
	act = new Queue<ColaActivado::message>(path, (long) QueueIdentifier::ACTIVADO, owner, false);
	act->get();
	act->remove();
	arm = new Queue<ColaArmado::message>(path, (long) QueueIdentifier::ARMADO, owner, false);
	arm->get();
	arm->remove();
	disp = new Queue<ColaDispositivo::message>(path, (long) QueueIdentifier::DISPOSITIVOS, owner, false);
	disp->get();
	disp->remove();
	sal = new Queue<ColaSalida::message>(path, (long) QueueIdentifier::SALIDA, owner, false);
	sal->get();
	sal->remove();

	mutexPlat = new Semaphore(path, (int) (long) SemaphoreIdentifier::MUTEX_PLATAFORMA, owner, false);
	mutexPlat->get();
	mutexPlat->remove();
	mutexPlat->post();
	semEspera = new SemaphoreArray(path, (int) (long) SemaphoreIdentifier::SEM_ESPERA, ROBOT_AMOUNT, owner, false);
	semEspera->get();
	semEspera->remove();
	shmP = new SharedMemory<ColaPlataforma::shared>(path, (int) (long) SharedMemoryIdentifier::PLATAFORMA, owner, false);
	shmP->get();
	shmP->remove();

	mutexExcl = new Semaphore(path, (int) (long) SemaphoreIdentifier::MUTEX_EXCLUSION, owner, false);
	mutexExcl->get();
	mutexExcl->remove();
	mutexExcl->post();
	semExcl = new SemaphoreArray(path, (int) (long) SemaphoreIdentifier::SEM_EXCLUSION, ROBOT_AMOUNT, owner, false);
	semExcl->get();
	semExcl->remove();
	shmE = new SharedMemory<ColaExclusion::shared>(path, (int) (long) SharedMemoryIdentifier::EXCLUSION, owner, false);
	shmE->get();
	shmE->remove();
}
