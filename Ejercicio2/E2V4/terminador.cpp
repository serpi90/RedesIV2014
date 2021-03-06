#include <string>

#include "includes.h"
#include "net-idManagerProtocol.h"
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
	Queue<Net::interfaceMessage> * net;

	Queue<IdManager::messageRequest> * req;
	Queue<IdManager::messageReply> * rep;

	Queue<Broker::message> * brk;
	Queue<Broker::outgoingMessage> * brko;

	Queue<ColaPlataforma::syncMessage> * syncPlat;

	Queue<IdManager::messageRequest> * idmReq;

	SharedMemory<Broker::tokenShm> * shmToken;
	Semaphore * s;
	s = new Semaphore(IPC::path, (int) IPC::SemaphoreIdentifier::MUTEX_TOKEN, owner, false);
	s->get();
	s->remove();
	s = new Semaphore(IPC::path, (int) IPC::SemaphoreIdentifier::TENGO_TOKEN, owner, false);
	s->get();
	s->remove();
	s = new Semaphore(IPC::path, (int) IPC::SemaphoreIdentifier::DEVUELVO_TOKEN, owner, false);
	s->get();
	s->remove();

	shmToken = new SharedMemory<Broker::tokenShm>(IPC::path, (int) IPC::SharedMemoryIdentifier::BROKER_TOKEN, owner, false);
	shmToken->get();
	shmToken->remove();

	idmReq = new Queue<IdManager::messageRequest>(IPC::path, (int) IPC::QueueIdentifier::ID_MANAGER_BROKER, owner, false);
	idmReq->get();
	idmReq->remove();

	Semaphore * mutexBrk;

	net = new Queue<Net::interfaceMessage>(IPC::path, (int) IPC::QueueIdentifier::TO_BROKER_FROM_BROKER, owner, false);
	net->get();
	net->remove();

	mutexBrk = new Semaphore(IPC::path, (int) IPC::SemaphoreIdentifier::MUTEX_BROKER_SYNC, owner, false);
	mutexBrk->get();
	mutexBrk->remove();
	SharedMemory<ColaPlataforma::shared> * shm;
	shm = new SharedMemory<ColaPlataforma::shared>(IPC::path, (int) IPC::SharedMemoryIdentifier::BROKER_PLAT, owner, false);
	shm->get();
	shm->remove();

	brk = new Queue<Broker::message>(IPC::path, (int) IPC::QueueIdentifier::TO_BROKER, owner, false);
	brk->get();
	brk->remove();
	brk = new Queue<Broker::message>(IPC::path, (int) IPC::QueueIdentifier::TO_BROKER_FROM_RECEIVER, owner, false);
	brk->get();
	brk->remove();
	brko = new Queue<Broker::outgoingMessage>(IPC::path, (int) IPC::QueueIdentifier::TO_SENDER_FROM_BROKER, owner, false);
	brko->get();
	brko->remove();

	req = new Queue<IdManager::messageRequest>(IPC::path, (int) IPC::QueueIdentifier::ID_MANAGER_FROM_INTERFACE_TO_WRAPPER, owner, false);
	req->get();
	req->remove();

	rep = new Queue<IdManager::messageReply>(IPC::path, (int) IPC::QueueIdentifier::ID_MANAGER_FROM_UNWRAPPER_TO_INTERFACE, owner, false);
	rep->get();
	rep->remove();

	Semaphore * mutexPlat;
	SemaphoreArray * semEspera;

	SharedMemory<ColaPlataforma::shared> * shmP;

	Semaphore * mutexExcl;
	SemaphoreArray * semExcl;

	SharedMemory<ColaExclusion::shared> * shmE;

	Semaphore * mutexIdm = new Semaphore(IPC::path, (int) IPC::SemaphoreIdentifier::MUTEX_ID_MANAGER, owner, false);
	mutexIdm->get();
	mutexIdm->remove();

	net = new Queue<Net::interfaceMessage>(IPC::path, (int) IPC::QueueIdentifier::FROM_WRAPPER_TO_NET, owner, false);
	net->get();
	net->remove();

	net = new Queue<Net::interfaceMessage>(IPC::path, (int) IPC::QueueIdentifier::FROM_NET_TO_UNWRAPPER, owner, false);
	net->get();
	net->remove();

	arm = new Queue<ColaArmado::message>(IPC::path, (int) IPC::QueueIdentifier::ARMADO_FROM_DISP_TO_WRAPPER, owner, false);
	arm->get();
	arm->remove();
	arm = new Queue<ColaArmado::message>(IPC::path, (int) IPC::QueueIdentifier::ARMADO_FROM_UNWRAPPER_TO_INTERFACE, owner, false);
	arm->get();
	arm->remove();
	arm = new Queue<ColaArmado::message>(IPC::path, (int) IPC::QueueIdentifier::ARMADO_BROKER, owner, false);
	arm->get();
	arm->remove();

	act = new Queue<ColaActivado::message>(IPC::path, (int) IPC::QueueIdentifier::ACTIVADO_FROM_DISP_TO_WRAPPER, owner, false);
	act->get();
	act->remove();
	act = new Queue<ColaActivado::message>(IPC::path, (int) IPC::QueueIdentifier::ACTIVADO_FROM_UNRWAPPER_TO_INTERFACE, owner, false);
	act->get();
	act->remove();
	act = new Queue<ColaActivado::message>(IPC::path, (int) IPC::QueueIdentifier::ACTIVADO_BROKER, owner, false);
	act->get();
	act->remove();

	sal = new Queue<ColaSalida::message>(IPC::path, (int) IPC::QueueIdentifier::SALIDA_FROM_INTERFACE_TO_WRAPPER, owner, false);
	sal->get();
	sal->remove();
	sal = new Queue<ColaSalida::message>(IPC::path, (int) IPC::QueueIdentifier::SALIDA_FROM_UNWRAPPER_TO_INTERFACE, owner, false);
	sal->get();
	sal->remove();
	sal = new Queue<ColaSalida::message>(IPC::path, (int) IPC::QueueIdentifier::SALIDA_BROKER, owner, false);
	sal->get();
	sal->remove();

	disp = new Queue<ColaDispositivo::message>(IPC::path, (int) IPC::QueueIdentifier::DISPOSITIVOS_FROM_PLATAFORMA_TO_WRAPPER, owner, false);
	disp->get();
	disp->remove();
	disp = new Queue<ColaDispositivo::message>(IPC::path, (int) IPC::QueueIdentifier::DISPOSITIVOS_FROM_UNWRAPPER_TO_DISP, owner, false);
	disp->get();
	disp->remove();
	disp = new Queue<ColaDispositivo::message>(IPC::path, (int) IPC::QueueIdentifier::DISPOSITIVOS_BROKER, owner, false);
	disp->get();
	disp->remove();

	plat = new Queue<ColaPlataforma::message>(IPC::path, (int) IPC::QueueIdentifier::FROM_INTERFACE_TO_PLATAFORMA, owner, false);
	plat->get();
	plat->remove();
	plat = new Queue<ColaPlataforma::message>(IPC::path, (int) IPC::QueueIdentifier::FROM_PLATAFORMA_TO_INTERFACE, owner, false);
	plat->get();
	plat->remove();

	syncPlat = new Queue<ColaPlataforma::syncMessage>(IPC::path, (int) IPC::QueueIdentifier::PLATAFORMA_TO_WRAPPER, owner, false);
	syncPlat->get();
	syncPlat->remove();
	syncPlat = new Queue<ColaPlataforma::syncMessage>(IPC::path, (int) IPC::QueueIdentifier::PLATAFORMA_FROM_UNWRAPPER, owner, false);
	syncPlat->get();
	syncPlat->remove();
	syncPlat = new Queue<ColaPlataforma::syncMessage>(IPC::path, (int) IPC::QueueIdentifier::PLATAFORMA_BROKER, owner, false);
	syncPlat->get();
	syncPlat->remove();

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

	system("pkill -9 net-idManager");
	system("pkill -9 net-receiver");
	system("pkill -9 broker-receiver");
	system("pkill -9 broker-sender");
	system("rm -f ids.dat broker.dat");
}
