#include "includes.h"
#include "Queue.cpp"
#include "Semaphore.h"
#include "SemaphoreArray.h"
#include "SharedMemory.cpp"
#include "Helper.h"
#include "Config.h"
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <sys/signal.h>
#include <strings.h>

using namespace plataforma;

enum class SlotStatus {
	RESERVED, OCCUPIED, FREE
};

enum class RobotStatus {
	WAITING, NOT_WAITING
};

struct shared {

		struct {
				struct dispositivo dispositivo;
				SlotStatus status;
		} slot[PLATFORM_CAPACITY];
		RobotStatus robotStatus[ROBOT_AMOUNT];
		unsigned amount;
};

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
		Queue<colaActivado::message> * colaDeActivado;
		Queue<colaDispositivo::message> * colaDeDispositivo;
};

Plataforma::Plataforma() {
	mutex = new Semaphore(IPC::path, (int) IPC::SemaphoreIdentifier::MUTEX_PLATAFORMA, "Plataforma");
	mutex->get();
	semEspera = new SemaphoreArray(IPC::path, (int) IPC::SemaphoreIdentifier::SEM_ESPERA, ROBOT_AMOUNT, "Plataforma");
	semEspera->get();
	shm = new SharedMemory<struct shared>(IPC::path, (int) IPC::SharedMemoryIdentifier::PLATAFORMA, "Plataforma");
	shm->get();
	SHM = shm->attach();
	colaDeActivado = new Queue<colaActivado::message>(IPC::path, (int) IPC::QueueIdentifier::ACTIVADO, "Plataforma");
	colaDeActivado->get();
	colaDeDispositivo = new Queue<colaDispositivo::message>(IPC::path, (int) IPC::QueueIdentifier::DISPOSITIVO, "Plataforma");
	colaDeDispositivo->get();
}

unsigned Plataforma::reservar(unsigned i) {
	mutex->wait();
	// TODO poner en diagrama
	while (SHM->amount == PLATFORM_CAPACITY) {
		SHM->robotStatus[i] = RobotStatus::WAITING;
		mutex->post();
		semEspera->wait(i);
		mutex->wait();
	}
	for (unsigned i; i < PLATFORM_CAPACITY; i++) {
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
	colaDispositivo::message m;
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

struct dispositivo Plataforma::esperarDispositivo() {
	colaActivado::message m;
	m = colaDeActivado->receive((long) IPC::MessageTypes::M_ANY);
	return m.dispositivo;
}

struct dispositivo Plataforma::tomarDispositivo(struct dispositivo dispositivo) {
	mutex->wait();
	for (unsigned i = 0; i < PLATFORM_CAPACITY; i++) {
		if (SHM->slot[i].dispositivo.id == dispositivo.id) {
			mutex->post();
			return dispositivo;
		}
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

	// Are we evil programmers that don't care about our processes children?
	// Or are we good programmers preventing the awakening of zombies?
	struct sigaction sigchld_action;
	sigchld_action.sa_handler = SIG_DFL;
	sigchld_action.sa_flags = SA_NOCLDWAIT;
	sigaction(SIGCHLD, &sigchld_action, NULL);

	fromInterface = new Queue<message>(IPC::path, (int) IPC::QueueIdentifier::PLATAFORMA_FROM_INTERFACE, "Plataforma");
	fromInterface->get();
	toInterface = new Queue<message>(IPC::path, (int) IPC::QueueIdentifier::PLATAFORMA_TO_INTERFACE, "Plataforma");
	toInterface->get();
	message m;

	while (true) {
		m = fromInterface->receive((long) IPC::MessageTypes::M_ANY);
		pid = fork();
		if (pid < 0) {
			perror("fork Plataforma");
			exit(EXIT_FAILURE);
		} else if (pid == 0) {
			switch (m.operation) {
				case RESERVAR:
					m.lugar = p.reservar(m.numero);
					break;
				case COLOCAR_DISPOSITIVO:
					p.colocarDispositivo(m.dispositivo, m.lugar);
					break;
				case ESPERAR_DISPOSITIVO:
					m.dispositivo = p.esperarDispositivo();
					break;
				case TOMAR_DISPOSITIVO:
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
