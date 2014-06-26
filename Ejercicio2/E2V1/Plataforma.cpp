#include <strings.h>
#include <unistd.h>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#include "Helper.h"
#include "includes.h"
#include "net-idManagerProtocol.h"
#include "Queue.cpp"
#include "Semaphore.h"
#include "SemaphoreArray.h"
#include "SharedMemory.cpp"

using namespace ColaPlataforma;

class Plataforma {
	public:
		Plataforma();
		unsigned reservar(unsigned);
		void colocarDispositivo(struct dispositivo, unsigned);
		struct dispositivo esperarDispositivo();
		struct dispositivo tomarDispositivo(struct dispositivo);
	private:
		long id;
		Semaphore * mutex;
		SemaphoreArray * semEspera;
		SharedMemory<shared> * shm;
		shared * SHM;
		Queue<ColaDispositivo::message> * colaDeDispositivo;
		Queue<ColaPlataforma::syncMessage> * toBroker, *fromBroker;
		Queue<Broker::message> * brokerRequests;
		void traerShm();
		void devolverShm();
};

Plataforma::Plataforma() {
	std::string owner = "Plataforma";
	Queue<IdManager::messageRequest> * toIdManager;
	IdManager::messageRequest idRequest;
	Queue<IdManager::messageReply> * fromIdManager;
	IdManager::messageReply idReply;

	toIdManager = new Queue<IdManager::messageRequest>(IPC::path, (int) IPC::QueueIdentifier::TO_ID_MANAGER, owner);
	toIdManager->get();
	fromIdManager = new Queue<IdManager::messageReply>(IPC::path, (int) IPC::QueueIdentifier::FROM_ID_MANAGER, owner);
	fromIdManager->get();

	idRequest.mtype = (long) IPC::MessageTypes::PLATAFORMA;
	idRequest.kind = IdManager::HostKind::PLATAFORMA;
	toIdManager->send(idRequest);
	idReply = fromIdManager->receive((long) IPC::MessageTypes::PLATAFORMA);

	this->id = idReply.id;

	mutex = new Semaphore(IPC::path, (int) IPC::SemaphoreIdentifier::MUTEX_PLATAFORMA, owner);
	mutex->get();
	semEspera = new SemaphoreArray(IPC::path, (int) IPC::SemaphoreIdentifier::SEM_ESPERA, ROBOT_AMOUNT, owner);
	semEspera->get();
	shm = new SharedMemory<shared>(IPC::path, (int) IPC::SharedMemoryIdentifier::PLATAFORMA, owner);
	shm->get();
	SHM = shm->attach();
	colaDeDispositivo = new Queue<ColaDispositivo::message>(IPC::path, (int) IPC::QueueIdentifier::DISPOSITIVOS_FROM_PLATAFORMA_TO_CTL, owner);
	colaDeDispositivo->get();
	toBroker = new Queue<ColaPlataforma::syncMessage>(IPC::path, (int) IPC::QueueIdentifier::PLATAFORMA_TO_BROKER, owner);
	toBroker->get();
	fromBroker = new Queue<ColaPlataforma::syncMessage>(IPC::path, (int) IPC::QueueIdentifier::PLATAFORMA_FROM_BROKER, owner);
	fromBroker->get();
	brokerRequests = new Queue<Broker::message>(IPC::path, (int) IPC::QueueIdentifier::TO_BROKER, owner);
	brokerRequests->get();
}

void Plataforma::traerShm() {
	std::stringstream ss;
	mutex->wait();
	Broker::message request;
	ColaPlataforma::syncMessage msg;

	//Solicito la shm
	request.request = Broker::Request::DAME_SHM;
	request.mtype = id;
	brokerRequests->send(request);

	// Recibo la shm
	msg = fromBroker->receive(id);
	*SHM = msg.shm;

	ss << "amount: " << SHM->amount;
	for (unsigned i = 0; i < ROBOT_AMOUNT; i++) {
		ss << " status " << i;
		switch (SHM->robotStatus[i]) {
			case RobotStatus::NOT_WAITING:
				ss << " NOT_WAITING";
				break;
			case RobotStatus::WAITING:
				ss << " WAITING";
				break;
		}
	}
	ss << std::endl;
	for (unsigned i = 0; i < PLATFORM_CAPACITY; i++) {
		ss << i << ": ";
		switch (SHM->slot[i].status) {
			case ColaPlataforma::SlotStatus::FREE:
				ss << "FREE ";
				break;
			case ColaPlataforma::SlotStatus::RESERVED:
				ss << "RSVD ";
				break;
			case ColaPlataforma::SlotStatus::OCCUPIED:
				ss << "OCPD ";
				break;
		}
	}
	ss << std::endl;
	Helper::output(stdout, ss);
}
void Plataforma::devolverShm() {
	ColaPlataforma::syncMessage msg;
	msg.shm = *SHM;
	msg.mtype = id;
	toBroker->send(msg);
	mutex->post();
}

unsigned Plataforma::reservar(unsigned i) {
	if (i >= ROBOT_AMOUNT) {
		Helper::output(stderr, "Plataforma: i >= ROBOT_AMOUNT.\n", Helper::Colours::RED);
		return -1;
	}

	this->traerShm();
	// TODO poner en diagrama
	while (SHM->amount == PLATFORM_CAPACITY) {
		SHM->robotStatus[i] = RobotStatus::WAITING;
		this->devolverShm();
		semEspera->wait(i);
		this->traerShm();
	}
	for (unsigned i = 0; i < PLATFORM_CAPACITY; i++) {
		if (SHM->slot[i].status == SlotStatus::FREE) {
			SHM->slot[i].status = SlotStatus::RESERVED;
			SHM->amount++;
			this->devolverShm();
			Helper::output(stdout, "plataforma termine RESERVAR\n", Helper::Colours::D_RED);
			return i;
		}
	}
	this->devolverShm();
	Helper::output(stderr, "Plataforma: no hay slots disponibles, cuando deberia haber.\n", Helper::Colours::RED);
	return -1;
}

void Plataforma::colocarDispositivo(struct dispositivo dispositivo, unsigned lugar) {
	ColaDispositivo::message m;
	std::stringstream ss;
	if (lugar >= PLATFORM_CAPACITY) {
		Helper::output(stderr, "Plataforma: lugar >= PLATFORM_CAPACITY.\n", Helper::Colours::RED);
		return;
	}

	this->traerShm();
	if (SHM->slot[lugar].status != SlotStatus::RESERVED) {
		this->devolverShm();
		ss << "Plataforma intentando colocar en un lugar no reservado: " << lugar << " ";
		switch (SHM->slot[lugar].status) {
			case SlotStatus::FREE:
				ss << "FREE" << std::endl;
				break;
			case SlotStatus::RESERVED:
				ss << "RESERVED" << std::endl;
				break;
			case SlotStatus::OCCUPIED:
				ss << "OCCUPIED" << std::endl;
				break;
		}
		Helper::output(stderr, ss, Helper::Colours::RED);
	} else {
		SHM->slot[lugar].dispositivo = dispositivo;
		SHM->slot[lugar].status = SlotStatus::OCCUPIED;
		this->devolverShm();
		// TODO: Actualizar diagrama
		m.mtype = dispositivo.id;
		Helper::output(stdout, "plataforma termine COLOCAR_DISPOSITIVO\n", Helper::Colours::D_RED);
		colaDeDispositivo->send(m);

	}
}

struct dispositivo Plataforma::tomarDispositivo(struct dispositivo dispositivo) {
	this->traerShm();
	for (unsigned i = 0; i < PLATFORM_CAPACITY; i++) {
		if (SHM->slot[i].dispositivo.id == dispositivo.id) {
			bzero(&(SHM->slot[i].dispositivo), sizeof(struct dispositivo));
			SHM->slot[i].status = SlotStatus::FREE;
			SHM->amount--;
			if (SHM->amount == PLATFORM_CAPACITY - 1) {
				for (unsigned j = 0; j < ROBOT_AMOUNT; j++) {
					if (SHM->robotStatus[i] == RobotStatus::WAITING) {
						SHM->robotStatus[i] = RobotStatus::NOT_WAITING;
						semEspera->post(i);
					}
				}
			}
		}
		this->devolverShm();
		Helper::output(stdout, "plataforma termine TOMAR_DISPOSITIVO\n", Helper::Colours::D_RED);
		return dispositivo;
	}
	this->devolverShm();
	Helper::output(stderr, "Plataforma: Se solicito un dispositivo inexistente.\n", Helper::Colours::RED);
	bzero(&dispositivo, sizeof(dispositivo));
	return dispositivo;
}

int main() {

	Plataforma p;
	pid_t pid;
	Queue<message> * fromInterface, *toInterface;
	Helper::Colours outputColor = Helper::Colours::D_RED;

// Are we evil programmers that don't care about our processes children?
// Or are we good programmers preventing the awakening of zombies?
	struct sigaction sigchld_action;
	sigchld_action.sa_handler = SIG_DFL;
	sigchld_action.sa_flags = SA_NOCLDWAIT;
	sigaction(SIGCHLD, &sigchld_action, NULL);

	fromInterface = new Queue<message>(IPC::path, (int) IPC::QueueIdentifier::FROM_INTERFACE_TO_PLATAFORMA, "Plataforma");
	fromInterface->get();
	toInterface = new Queue<message>(IPC::path, (int) IPC::QueueIdentifier::FROM_PLATAFORMA_TO_INTERFACE, "Plataforma");
	toInterface->get();
	message m;

	while (true) {
		m = fromInterface->receive((long) IPC::MessageTypes::ANY);
		pid = fork();
		if (pid < 0) {
			perror("fork Plataforma");
			exit(EXIT_FAILURE);
		} else if (pid == 0) {
			switch (m.operation) {
				case RESERVAR:
					Helper::output(stdout, "plataforma recibi RESERVAR\n", outputColor);
					m.lugar = p.reservar(m.numero);
					break;
				case COLOCAR_DISPOSITIVO:
					Helper::output(stdout, "plataforma recibi COLOCAR DISPOSITIVO\n", outputColor);
					p.colocarDispositivo(m.dispositivo, m.lugar);
					break;
				case TOMAR_DISPOSITIVO:
					Helper::output(stdout, "plataforma recibi TOMAR DISPOSITIVO\n", outputColor);
					m.dispositivo = p.tomarDispositivo(m.dispositivo);
					break;
				default:
					return EXIT_FAILURE;
			}
			toInterface->send(m);
			return EXIT_SUCCESS;
		}
	}
	return EXIT_SUCCESS;
}
