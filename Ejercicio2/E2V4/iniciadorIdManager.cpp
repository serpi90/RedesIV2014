#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <string>

#include "includes.h"
#include "Semaphore.h"

using namespace IPC;

int main() {
	std::string owner = "iniciador id manager";

	pid_t pid;
	Semaphore * mutexIdm = new Semaphore(IPC::path, (int) IPC::SemaphoreIdentifier::MUTEX_ID_MANAGER, owner);
	mutexIdm->create();
	mutexIdm->post();

	pid = fork();
	if (pid == 0) {
		execlp("./net-idManager", "net-idManager", NULL);
		perror("net-idManager - execlp: ");
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		perror("net-idManager - fork: ");
	}
}
