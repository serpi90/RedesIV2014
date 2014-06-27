#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <string>

#include "includes.h"
#include "net-idManagerProtocol.h"
#include "Queue.cpp"
#include "Semaphore.h"
#include "SharedMemory.cpp"

using namespace IPC;

int main(int argc, char * argv[]) {
	std::string owner = "iniciador broker";

	pid_t pid;
	Queue<ColaActivado::message> * act;
	Queue<ColaArmado::message> * arm;
	Queue<ColaDispositivo::message> * disp;
	Queue<ColaSalida::message> * sal;
	Queue<Broker::message> * brk;
	Queue<Broker::outgoingMessage> * brko;
	Queue<ColaPlataforma::syncMessage> * syncPlat;
	Queue<IdManager::messageRequest> * idmReq;
	Queue<Net::interfaceMessage> * toBroker;
	Semaphore * mutex;
	SharedMemory<ColaPlataforma::shared> * shm;

	SharedMemory<Broker::tokenShm> * shmToken;
	Semaphore * s;
	s = new Semaphore(IPC::path, (int) IPC::SemaphoreIdentifier::MUTEX_TOKEN, owner);
	s->create();
	s->post();
	s = new Semaphore(IPC::path, (int) IPC::SemaphoreIdentifier::TENGO_TOKEN, owner);
	s->create();
	s = new Semaphore(IPC::path, (int) IPC::SemaphoreIdentifier::DEVUELVO_TOKEN, owner);
	s->create();

	shmToken = new SharedMemory<Broker::tokenShm>(IPC::path, (int) IPC::SharedMemoryIdentifier::BROKER_TOKEN, owner);
	shmToken->create();
	Broker::tokenShm * ts = shmToken->attach();
	ts->necesitoToken = 0;

	toBroker = new Queue<Net::interfaceMessage>(IPC::path, (int) IPC::QueueIdentifier::TO_BROKER_FROM_BROKER, owner);
	toBroker->create();

	idmReq = new Queue<IdManager::messageRequest>(IPC::path, (int) IPC::QueueIdentifier::ID_MANAGER_BROKER, owner);
	idmReq->create();

	shm = new SharedMemory<ColaPlataforma::shared>(IPC::path, (int) IPC::SharedMemoryIdentifier::BROKER_PLAT, owner);
	shm->create();

	mutex = new Semaphore(IPC::path, (int) IPC::SemaphoreIdentifier::MUTEX_BROKER_SYNC, owner);
	mutex->create();
	mutex->post();

	brk = new Queue<Broker::message>(IPC::path, (int) IPC::QueueIdentifier::TO_BROKER_FROM_RECEIVER, owner);
	brk->create();

	brko = new Queue<Broker::outgoingMessage>(IPC::path, (int) IPC::QueueIdentifier::TO_SENDER_FROM_BROKER, owner);
	brko->create();

	arm = new Queue<ColaArmado::message>(IPC::path, (int) IPC::QueueIdentifier::ARMADO_BROKER, owner);
	arm->create();

	act = new Queue<ColaActivado::message>(IPC::path, (int) IPC::QueueIdentifier::ACTIVADO_BROKER, owner);
	act->create();

	sal = new Queue<ColaSalida::message>(IPC::path, (int) IPC::QueueIdentifier::SALIDA_BROKER, owner);
	sal->create();

	disp = new Queue<ColaDispositivo::message>(IPC::path, (int) IPC::QueueIdentifier::DISPOSITIVOS_BROKER, owner);
	disp->create();

	syncPlat = new Queue<ColaPlataforma::syncMessage>(IPC::path, (int) IPC::QueueIdentifier::PLATAFORMA_BROKER, owner);
	syncPlat->create();

	pid = fork();
	if (pid == 0) {
		execlp("./broker-sender", "broker-sender", NULL);
		perror("broker-sender - execlp: ");
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		perror("broker-sender - fork: ");
	}

	pid = fork();
	if (pid == 0) {
		execlp("./broker-receiver", "broker-receiver", NULL);
		perror("broker-receiver - execlp: ");
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		perror("broker-receiver - fork: ");
	}

	if (argc > 1) {
		pid = fork();
		if (pid == 0) {
			execlp("./broker", "broker", argv[1], NULL);
			perror("broker - execlp: ");
			exit(EXIT_FAILURE);
		} else if (pid < 0) {
			perror("broker - fork: ");
		}
	} else {
		pid = fork();
		if (pid == 0) {
			execlp("./broker", "broker", NULL);
			perror("broker - execlp: ");
			exit(EXIT_FAILURE);
		} else if (pid < 0) {
			perror("broker - fork: ");
		}
	}
	return 0;
}
