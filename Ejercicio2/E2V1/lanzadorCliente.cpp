#include <unistd.h>
#include <cstdio>
#include <cstdlib>

using namespace std;

int main() {
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
		execlp("./net-idManager-forwarder", "net-idManager-forwarder", NULL);
		perror("net-idManager-forwarder - execlp: ");
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		perror("net-idManager-forwarder - fork: ");
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
