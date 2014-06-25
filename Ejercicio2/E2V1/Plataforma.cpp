#include <strings.h>
#include <unistd.h>
#include <csignal>
#include <cstdio>
#include <cstdlib>

#include "Helper.h"
#include "includes.h"
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
		Semaphore * mutex;
		SemaphoreArray * semEspera;
		SharedMemory<struct shared> * shm;
		struct shared * SHM;
		Queue<ColaDispositivo::message> * colaDeDispositivo;
};

Plataforma::Plataforma() {
	mutex = new Semaphore(IPC::path, (int) IPC::SemaphoreIdentifier::MUTEX_PLATAFORMA, "Plataforma");
	mutex->get();
	semEspera = new SemaphoreArray(IPC::path, (int) IPC::SemaphoreIdentifier::SEM_ESPERA, ROBOT_AMOUNT, "Plataforma");
	semEspera->get();
	shm = new SharedMemory<struct shared>(IPC::path, (int) IPC::SharedMemoryIdentifier::PLATAFORMA, "Plataforma");
	shm->get();
	SHM = shm->attach();
	colaDeDispositivo = new Queue<ColaDispositivo::message>(IPC::path, (int) IPC::QueueIdentifier::DISPOSITIVOS_FROM_PLATAFORMA_TO_CTL, "Plataforma");
	colaDeDispositivo->get();
}

unsigned Plataforma::reservar(unsigned i) {
	if (i >= ROBOT_AMOUNT) {
		Helper::output(stderr, "Plataforma: i >= ROBOT_AMOUNT.\n", Helper::Colours::RED);
		return -1;
	}
	mutex->wait();
	// TODO poner en diagrama
	while (SHM->amount == PLATFORM_CAPACITY) {
		SHM->robotStatus[i] = RobotStatus::WAITING;
		mutex->post();
		semEspera->wait(i);
		mutex->wait();
	}
	for (unsigned i = 0; i < PLATFORM_CAPACITY; i++) {
		if (SHM->slot[i].status == SlotStatus::FREE) {
			SHM->slot[i].status = SlotStatus::RESERVED;
			SHM->amount++;
			mutex->post();
			return i;
		}
	}
	mutex->post();
	Helper::output(stderr, "Plataforma: no hay slots disponibles, cuando deberia haber.\n", Helper::Colours::RED);
	return -1;
}

void Plataforma::colocarDispositivo(struct dispositivo dispositivo, unsigned lugar) {
	ColaDispositivo::message m;
	if (lugar >= PLATFORM_CAPACITY) {
		Helper::output(stderr, "Plataforma: lugar >= PLATFORM_CAPACITY.\n", Helper::Colours::RED);
		return;
	}
	mutex->wait();
	if (SHM->slot[lugar].status != SlotStatus::RESERVED) {
		mutex->post();
		Helper::output(stderr, "Plataforma intentando reservar en un lugar ocupado.\n", Helper::Colours::RED);
		return;
	}
	SHM->slot[lugar].dispositivo = dispositivo;
	SHM->slot[lugar].status = SlotStatus::OCCUPIED;
	mutex->post();
	// TODO: Actualizar diagrama
	m.mtype = dispositivo.id;
	colaDeDispositivo->send(m);
}

struct dispositivo Plataforma::tomarDispositivo(struct dispositivo dispositivo) {
	mutex->wait();
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
		mutex->post();
		return dispositivo;
	}
	mutex->post();
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
					Helper::output(stdout, "RESERVAR\n", outputColor);
					m.lugar = p.reservar(m.numero);
					break;
				case COLOCAR_DISPOSITIVO:
					Helper::output(stdout, "COLOCAR DISPOSITIVO\n", outputColor);
					p.colocarDispositivo(m.dispositivo, m.lugar);
					break;
				case TOMAR_DISPOSITIVO:
					Helper::output(stdout, "TOMAR DISPOSITIVO\n", outputColor);
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
