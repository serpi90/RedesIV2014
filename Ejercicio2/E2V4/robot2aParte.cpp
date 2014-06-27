#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <string>

#include "Helper.h"
#include "includes.h"
#include "iRobot2aParte.h"

int main(int argc, char * argv[]) {
	if (argc < 2) {
		Helper::output(stderr, "Usage: robot1aParteSacado <tipo>\n", Helper::Colours::RED);
		exit(EXIT_FAILURE);
		Helper::output(stderr, IPC::path);
	}

	std::string owner;
	long me;

	iRobot2aParte * interfaz;
	struct dispositivo dispositivo;
	std::stringstream ss;
	const Helper::Colours outputColour = Helper::Colours::YELLOW;

	me = atol(argv[1]);
	owner = std::string("robot2aParte ") + argv[1];

	interfaz = new iRobot2aParte(me);

	while (true) {
		Helper::output(stdout, owner + " esperando un dispositivo para sacar.\n", outputColour);
		dispositivo = interfaz->tomarDispositivo();
		ss << owner << " saque el dispositivo " << dispositivo.id << " de tipo " << dispositivo.tipo << std::endl;
		Helper::output(stdout, ss, Helper::Colours::GREEN);
	}

	return 0;
}
