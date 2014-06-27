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

using namespace ColaExclusion;

class Exclusion {
	public:
		Exclusion();
		void esperarSiSacando(unsigned);
		void avisarSiEsperandoParaSacar(unsigned);
		void esperarSiArmando(unsigned);
		void avisarSiEsperandoParaArmar(unsigned);
	private:
		Semaphore * mutex;
		SemaphoreArray * semExcl;
		SharedMemory<struct shared> * shm;
		struct shared * SHMExcl;
};

Exclusion::Exclusion() {
	mutex = new Semaphore(IPC::path, (int) IPC::SemaphoreIdentifier::MUTEX_EXCLUSION, "Exclusion");
	mutex->get();
	semExcl = new SemaphoreArray(IPC::path, (int) IPC::SemaphoreIdentifier::SEM_EXCLUSION, ROBOT_AMOUNT, "Exclusion");
	semExcl->get();
	shm = new SharedMemory<struct shared>(IPC::path, (int) IPC::SharedMemoryIdentifier::EXCLUSION, "Exclusion");
	shm->get();
	SHMExcl = shm->attach();
}

void Exclusion::esperarSiSacando(unsigned i) {
	if (i >= ROBOT_AMOUNT) {
		Helper::output(stderr, "ExclusionRobots: i >= ROBOT_AMOUNT.\n", Helper::Colours::RED);
		return;
	}
	mutex->wait();
	if (SHMExcl->sacando[i] == BUSY) {
		Helper::output(stdout, "espero\n", Helper::Colours::D_GREEN);
		SHMExcl->armando[i] = WAITING;
		mutex->post();
		semExcl->wait(i);
		mutex->wait();
	}
	Helper::output(stdout, "puedo armar\n", Helper::Colours::D_GREEN);
	SHMExcl->armando[i] = BUSY;
	mutex->post();
}

void Exclusion::avisarSiEsperandoParaSacar(unsigned i) {
	if (i >= ROBOT_AMOUNT) {
		Helper::output(stderr, "ExclusionRobots: i >= ROBOT_AMOUNT.\n", Helper::Colours::RED);
		return;
	}
	mutex->wait();
	SHMExcl->armando[i] = IDLE;
	if (SHMExcl->sacando[i] == WAITING) {
		mutex->post();
		semExcl->post(i);
	} else {
		mutex->post();
	}
}

void Exclusion::esperarSiArmando(unsigned i) {
	if (i >= ROBOT_AMOUNT) {
		Helper::output(stderr, "ExclusionRobots: i >= ROBOT_AMOUNT.\n", Helper::Colours::RED);
		return;
	}
	mutex->wait();
	if (SHMExcl->armando[i] == BUSY) {
		Helper::output(stdout, "espero\n", Helper::Colours::D_GREEN);
		SHMExcl->sacando[i] = WAITING;
		mutex->post();
		semExcl->wait(i);
		mutex->wait();
	}
	Helper::output(stdout, "puedo sacar\n", Helper::Colours::D_GREEN);
	SHMExcl->sacando[i] = BUSY;
	mutex->post();
}

void Exclusion::avisarSiEsperandoParaArmar(unsigned i) {
	if (i >= ROBOT_AMOUNT) {
		Helper::output(stderr, "ExclusionRobots: i >= ROBOT_AMOUNT.\n", Helper::Colours::RED);
		return;
	}
	mutex->wait();
	SHMExcl->sacando[i] = IDLE;
	if (SHMExcl->armando[i] == WAITING) {
		mutex->post();
		semExcl->post(i);
	} else {
		mutex->post();
	}
}

int main() {

	Exclusion e;
	pid_t pid;
	Queue<message> * fromInterface, *toInterface;
	Helper::Colours outputColor = Helper::Colours::D_GREEN;

	// Are we evil programmers that don't care abour our processes children?
	// Or are we good programmers preventing the awakening of zombies?
	struct sigaction sigchld_action;
	sigchld_action.sa_handler = SIG_DFL;
	sigchld_action.sa_flags = SA_NOCLDWAIT;
	sigaction(SIGCHLD, &sigchld_action, NULL);

	fromInterface = new Queue<message>(IPC::path, (int) IPC::QueueIdentifier::FROM_INTERFACE_TO_EXCLUSION, "Exclusion");
	fromInterface->get();
	toInterface = new Queue<message>(IPC::path, (int) IPC::QueueIdentifier::FROM_EXCLUSION_TO_INTERFACE, "Exclusion");
	toInterface->get();
	message m;

	while (true) {
		m = fromInterface->receive((long) IPC::MessageTypes::ANY);
		pid = fork();
		if (pid < 0) {
			perror("fork Exclusion");
			exit(EXIT_FAILURE);
		} else if (pid == 0) {
			switch (m.operation) {
				case ESPERAR_SI_SACANDO:
					Helper::output(stdout, "ExclusionRobots recibi ESPERAR SI SACANDO\n", outputColor);
					e.esperarSiSacando(m.number);
					toInterface->send(m);
					break;
				case AVISAR_SI_ESPERANDO_PARA_SACAR:
					Helper::output(stdout, "ExclusionRobots recibi AVISAR SI ESPERANDO PARA SACAR\n", outputColor);
					e.avisarSiEsperandoParaSacar(m.number);
					break;
				case ESPERAR_SI_ARMANDO:
					Helper::output(stdout, "ExclusionRobots recibi ESPERAR SI ARMANDO\n", outputColor);
					e.esperarSiArmando(m.number);
					toInterface->send(m);
					break;
				case AVISAR_SI_ESPERANDO_PARA_ARMAR:
					Helper::output(stdout, "ExclusionRobots recibi AVISAR SI ESPERANDO PARA ARMAR\n", outputColor);
					e.avisarSiEsperandoParaArmar(m.number);
					break;
				default:
					return EXIT_FAILURE;
			}
			return EXIT_SUCCESS;
		}
	}
	return EXIT_SUCCESS;
}
