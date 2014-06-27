#include <string>

#include "includes.h"
#include "net-idManagerProtocol.h"
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
	Queue<Net::interfaceMessage> * net;
	Queue<IdManager::messageRequest> * req;
	Queue<IdManager::messageReply> * rep;
	Queue<Broker::message> * brk;
	Queue<ColaPlataforma::syncMessage> * syncPlat;

	brk = new Queue<Broker::message>(IPC::path, (int) IPC::QueueIdentifier::TO_BROKER, owner);
	brk->create();

	req = new Queue<IdManager::messageRequest>(IPC::path, (int) IPC::QueueIdentifier::ID_MANAGER_FROM_INTERFACE_TO_WRAPPER, owner);
	req->create();

	rep = new Queue<IdManager::messageReply>(IPC::path, (int) IPC::QueueIdentifier::ID_MANAGER_FROM_UNWRAPPER_TO_INTERFACE, owner);
	rep->create();

	Semaphore * mutexPlat;
	SemaphoreArray * semEspera;

	SharedMemory<ColaPlataforma::shared> * shmP;
	ColaPlataforma::shared * plataforma;

	Semaphore * mutexExcl;
	SemaphoreArray * semExcl;

	SharedMemory<ColaExclusion::shared> * shmE;
	ColaExclusion::shared * exclusion;

	net = new Queue<Net::interfaceMessage>(IPC::path, (int) IPC::QueueIdentifier::FROM_WRAPPER_TO_NET, owner);
	net->create();

	net = new Queue<Net::interfaceMessage>(IPC::path, (int) IPC::QueueIdentifier::FROM_NET_TO_UNWRAPPER, owner);
	net->create();

	arm = new Queue<ColaArmado::message>(IPC::path, (int) IPC::QueueIdentifier::ARMADO_FROM_DISP_TO_WRAPPER, owner);
	arm->create();
	arm = new Queue<ColaArmado::message>(IPC::path, (int) IPC::QueueIdentifier::ARMADO_FROM_UNWRAPPER_TO_INTERFACE, owner);
	arm->create();

	act = new Queue<ColaActivado::message>(IPC::path, (int) IPC::QueueIdentifier::ACTIVADO_FROM_DISP_TO_WRAPPER, owner);
	act->create();
	act = new Queue<ColaActivado::message>(IPC::path, (int) IPC::QueueIdentifier::ACTIVADO_FROM_UNRWAPPER_TO_INTERFACE, owner);
	act->create();

	sal = new Queue<ColaSalida::message>(IPC::path, (int) IPC::QueueIdentifier::SALIDA_FROM_INTERFACE_TO_WRAPPER, owner);
	sal->create();
	sal = new Queue<ColaSalida::message>(IPC::path, (int) IPC::QueueIdentifier::SALIDA_FROM_UNWRAPPER_TO_INTERFACE, owner);
	sal->create();

	disp = new Queue<ColaDispositivo::message>(IPC::path, (int) IPC::QueueIdentifier::DISPOSITIVOS_FROM_PLATAFORMA_TO_WRAPPER, owner);
	disp->create();
	disp = new Queue<ColaDispositivo::message>(IPC::path, (int) IPC::QueueIdentifier::DISPOSITIVOS_FROM_UNWRAPPER_TO_DISP, owner);
	disp->create();

	plat = new Queue<ColaPlataforma::message>(IPC::path, (int) IPC::QueueIdentifier::FROM_INTERFACE_TO_PLATAFORMA, owner);
	plat->create();
	plat = new Queue<ColaPlataforma::message>(IPC::path, (int) IPC::QueueIdentifier::FROM_PLATAFORMA_TO_INTERFACE, owner);
	plat->create();

	syncPlat = new Queue<ColaPlataforma::syncMessage>(IPC::path, (int) IPC::QueueIdentifier::PLATAFORMA_TO_WRAPPER, owner);
	syncPlat->create();
	syncPlat = new Queue<ColaPlataforma::syncMessage>(IPC::path, (int) IPC::QueueIdentifier::PLATAFORMA_FROM_UNWRAPPER, owner);
	syncPlat->create();

	excl = new Queue<ColaExclusion::message>(IPC::path, (int) IPC::QueueIdentifier::FROM_INTERFACE_TO_EXCLUSION, owner);
	excl->create();
	excl = new Queue<ColaExclusion::message>(IPC::path, (int) IPC::QueueIdentifier::FROM_EXCLUSION_TO_INTERFACE, owner);
	excl->create();

	mutexPlat = new Semaphore(path, (int) SemaphoreIdentifier::MUTEX_PLATAFORMA, owner);
	mutexPlat->create();
	mutexPlat->post();
	semEspera = new SemaphoreArray(path, (int) SemaphoreIdentifier::SEM_ESPERA, ROBOT_AMOUNT, owner);
	semEspera->create();
	shmP = new SharedMemory<ColaPlataforma::shared>(path, (int) SharedMemoryIdentifier::PLATAFORMA, owner);
	shmP->create();
	plataforma = shmP->attach();
	plataforma->amount = 0;
	for (unsigned i = 0; i < ROBOT_AMOUNT; i++) {
		plataforma->robotStatus[i] = ColaPlataforma::RobotStatus::NOT_WAITING;
	}
	for (unsigned i = 0; i < PLATFORM_CAPACITY; i++) {
		plataforma->slot[i].status = ColaPlataforma::SlotStatus::FREE;
	}

	mutexExcl = new Semaphore(path, (int) SemaphoreIdentifier::MUTEX_EXCLUSION, owner);
	mutexExcl->create();
	mutexExcl->post();
	semExcl = new SemaphoreArray(path, (int) SemaphoreIdentifier::SEM_EXCLUSION, ROBOT_AMOUNT, owner);
	semExcl->create();
	shmE = new SharedMemory<ColaExclusion::shared>(path, (int) SharedMemoryIdentifier::EXCLUSION, owner);
	shmE->create();
	exclusion = shmE->attach();

	for (unsigned i = 0; i < ROBOT_AMOUNT; i++) {
		exclusion->armando[i] = ColaExclusion::RobotStatus::IDLE;
		exclusion->sacando[i] = ColaExclusion::RobotStatus::IDLE;
	}

	pid_t pid;

	pid = fork();
	if (pid == 0) {
		execlp("./net-sender", "net-sender", NULL);
		perror("net-sender - execlp: ");
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		perror("net-sender - fork: ");
	}

	pid = fork();
	if (pid == 0) {
		execlp("./net-unwrapper", "net-unwrapper", NULL);
		perror("net-unwrapper - execlp: ");
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		perror("net-unwrapper - fork: ");
	}
	pid = fork();
	if (pid == 0) {
		execlp("./net-wrapper-activado", "net-wrapper-activado", NULL);
		perror("net-wrapper-activado - execlp: ");
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		perror("net-wrapper-activado - fork: ");
	}
	pid = fork();
	if (pid == 0) {
		execlp("./net-wrapper-armado", "net-wrapper-armado", NULL);
		perror("net-wrapper-armado - execlp: ");
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		perror("net-wrapper-armado - fork: ");
	}
	pid = fork();
	if (pid == 0) {
		execlp("./net-wrapper-broker", "net-wrapper-broker", NULL);
		perror("net-wrapper-broker - execlp: ");
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		perror("net-wrapper-broker - fork: ");
	}
	pid = fork();
	if (pid == 0) {
		execlp("./net-wrapper-dispositivo", "net-wrapper-dispositivo", NULL);
		perror("net-wrapper-dispositivo - execlp: ");
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		perror("net-wrapper-dispositivo - fork: ");
	}
	pid = fork();
	if (pid == 0) {
		execlp("./net-wrapper-salida", "net-wrapper-salida", NULL);
		perror("net-wrapper-salida - execlp: ");
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		perror("net-wrapper-salida - fork: ");
	}

	pid = fork();
	if (pid == 0) {
		execlp("./net-wrapper-syncMessage", "net-wrapper-syncMessage", NULL);
		perror("net-wrapper-syncMessage - execlp: ");
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		perror("net-wrapper-syncMessage - fork: ");
	}

	pid = fork();
	if (pid == 0) {
		execlp("./net-wrapper-idm", "net-wrapper-idm", NULL);
		perror("net-wrapper-idm - execlp: ");
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		perror("net-wrapper-idm - fork: ");
	}

	pid = fork();
	if (pid == 0) {
		execlp("./Plataforma", "Plataforma", NULL);
		perror("Plataforma - execlp: ");
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		perror("Plataforma - fork: ");
	}

	pid = fork();
	if (pid == 0) {
		execlp("./ExclusionRobots", "ExclusionRobots", NULL);
		perror("ExclusionRobots - execlp: ");
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		perror("ExclusionRobots - fork: ");
	}
}
