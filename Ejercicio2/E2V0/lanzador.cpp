#include "includes.h"
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <sstream>

using namespace std;

int main(int argc, char* argv[]) {
	pid_t pid;

	pid = fork();
	if (pid == 0) {
		execlp("./program", "program", NULL);
		perror("program - execlp: ");
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		perror("program - fork: ");
	}
}
