#include "includes.h"
#include "Helper.h"
#include <unistd.h>
#include <sstream>

using namespace std;

int main(int argc, char* argv[]) {
	std::stringstream ss;
	pid_t pid;
	Helper::output(stdout, IPC::path);
	unsigned d;
	if (argc < 2) {
		d = 0;
	} else {
		d = atoi(argv[1]);
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
	for (unsigned i = ROBOT_AMOUNT; i > 0; i--) {
		ss << i;

		pid = fork();
		if (pid == 0) {
			execlp("./robot1aParteArmado", "robot1aParteArmado", ss.str().c_str(), NULL);
			perror("robot1aParteArmado - execlp: ");
			exit(EXIT_FAILURE);
		} else if (pid < 0) {
			perror("robot1aParteArmado - fork: ");
		}

		pid = fork();
		if (pid == 0) {
			execlp("./robot1aParteSacado", "robot1aParteSacado", ss.str().c_str(), NULL);
			perror("robot1aParteSacado - execlp: ");
			exit(EXIT_FAILURE);
		} else if (pid < 0) {
			perror("robot1aParteSacado - fork: ");
		}
		ss.str("");
	}

	for (unsigned i = DISPOSITIVE_TYPES; i > 0; i--) {
		ss << (i + 1);

		pid = fork();
		if (pid == 0) {
			execlp("./robot2aParte", "robot2aParte", ss.str().c_str(), NULL);
			perror("robot2aParte - execlp: ");
			exit(EXIT_FAILURE);
		} else if (pid < 0) {
			perror("robot2aParte - fork: ");
		}
		ss.str("");
	}

	for (unsigned i = d; i > 0; i--) {
		ss << i;
		pid = fork();
		if (pid == 0) {
			execlp("./dispositivo", "dispositivo", ss.str().c_str(), NULL);
			perror("dispositivo - execlp: ");
			exit(EXIT_FAILURE);
		} else if (pid < 0) {
			perror("dispositivo - fork: ");
		}
		ss.str("");
	}
}
