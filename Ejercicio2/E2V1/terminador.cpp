#include <string>

#include "includes.h"
#include "Queue.cpp"
#include "Semaphore.h"
#include "SemaphoreArray.h"
#include "SharedMemory.cpp"

using namespace IPC;

int main() {
	std::string owner = "terminador";

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

	Semaphore * mutexExcl;
	SemaphoreArray * semExcl;

	SharedMemory<ColaExclusion::shared> * shmE;

	Semaphore * mutexIdm = new Semaphore(IPC::path, (int) IPC::SemaphoreIdentifier::MUTEX_ID_MANAGER, owner, false);
	mutexIdm->get();
	mutexIdm->remove();

	net = new Queue<Net::iMessage>(IPC::path, (int) IPC::QueueIdentifier::FROM_CTL_TO_NET, owner, false);
	net->get();
	net->remove();

	arm = new Queue<ColaArmado::message>(IPC::path, (int) IPC::QueueIdentifier::ARMADO_FROM_DISP_TO_CTL, owner, false);
	arm->get();
	arm->remove();
	arm = new Queue<ColaArmado::message>(IPC::path, (int) IPC::QueueIdentifier::ARMADO_FROM_CTL_TO_INTERFACE, owner, false);
	arm->get();
	arm->remove();

	act = new Queue<ColaActivado::message>(IPC::path, (int) IPC::QueueIdentifier::ACTIVADO_FROM_DISP_TO_CTL, owner, false);
	act->get();
	act->remove();
	act = new Queue<ColaActivado::message>(IPC::path, (int) IPC::QueueIdentifier::ACTIVADO_FROM_CTL_TO_INTERFACE, owner, false);
	act->get();
	act->remove();

	sal = new Queue<ColaSalida::message>(IPC::path, (int) IPC::QueueIdentifier::SALIDA_FROM_INTERFACE_TO_CTL, owner, false);
	sal->get();
	sal->remove();
	sal = new Queue<ColaSalida::message>(IPC::path, (int) IPC::QueueIdentifier::SALIDA_FROM_CTL_TO_INTERFACE, owner, false);
	sal->get();
	sal->remove();

	disp = new Queue<ColaDispositivo::message>(IPC::path, (int) IPC::QueueIdentifier::DISPOSITIVOS_FROM_PLATAFORMA_TO_CTL, owner, false);
	disp->get();
	disp->remove();
	disp = new Queue<ColaDispositivo::message>(IPC::path, (int) IPC::QueueIdentifier::DISPOSITIVOS_FROM_CTL_TO_DISP, owner, false);
	disp->get();
	disp->remove();

	plat = new Queue<ColaPlataforma::message>(IPC::path, (int) IPC::QueueIdentifier::FROM_INTERFACE_TO_PLATAFORMA, owner, false);
	plat->get();
	plat->remove();
	plat = new Queue<ColaPlataforma::message>(IPC::path, (int) IPC::QueueIdentifier::FROM_PLATAFORMA_TO_INTERFACE, owner, false);
	plat->get();
	plat->remove();

	excl = new Queue<ColaExclusion::message>(IPC::path, (int) IPC::QueueIdentifier::FROM_INTERFACE_TO_EXCLUSION, owner, false);
	excl->get();
	excl->remove();
	excl = new Queue<ColaExclusion::message>(IPC::path, (int) IPC::QueueIdentifier::FROM_EXCLUSION_TO_INTERFACE, owner, false);
	excl->get();
	excl->remove();

	mutexPlat = new Semaphore(path, (int) SemaphoreIdentifier::MUTEX_PLATAFORMA, owner, false);
	mutexPlat->get();
	mutexPlat->remove();
	mutexPlat->post();
	semEspera = new SemaphoreArray(path, (int) SemaphoreIdentifier::SEM_ESPERA, ROBOT_AMOUNT, owner, false);
	semEspera->get();
	semEspera->remove();
	shmP = new SharedMemory<ColaPlataforma::shared>(path, (int) SharedMemoryIdentifier::PLATAFORMA, owner, false);
	shmP->get();
	shmP->remove();

	mutexExcl = new Semaphore(path, (int) SemaphoreIdentifier::MUTEX_EXCLUSION, owner, false);
	mutexExcl->get();
	mutexExcl->remove();
	mutexExcl->post();
	semExcl = new SemaphoreArray(path, (int) SemaphoreIdentifier::SEM_EXCLUSION, ROBOT_AMOUNT, owner, false);
	semExcl->get();
	semExcl->remove();
	shmE = new SharedMemory<ColaExclusion::shared>(path, (int) SharedMemoryIdentifier::EXCLUSION, owner, false);
	shmE->get();
	shmE->remove();
}
