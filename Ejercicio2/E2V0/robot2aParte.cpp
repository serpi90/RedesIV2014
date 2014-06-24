#include "includes.h"
#include "Queue.cpp"
#include "Helper.h"
#include <sstream>
#include <string>

int main(int argc, char * argv[]) {
	if (argc < 2) {
		Helper::output(stderr, "Usage: robot1aParteSacado <tipo>\n", Helper::Colours::RED);
		exit(EXIT_FAILURE);
	}

	std::string owner;
	long me;

	Queue<ColaSalida::message> * colaDeSalida;
	ColaSalida::message msgSalida;
	std::stringstream ss;
	const Helper::Colours outputColour = Helper::Colours::YELLOW;

	me = atol(argv[1]);
	owner = std::string("robot2aParte ") + argv[1];

	colaDeSalida = new Queue<ColaSalida::message>(IPC::path, (long) IPC::QueueIdentifier::SALIDA, owner);
	colaDeSalida->get();

	while (true) {
		Helper::output(stdout, owner + " esperando un dispositivo para sacar.\n", outputColour);
		msgSalida = colaDeSalida->receive(me);
		ss << owner << " saque el dispositivo " << msgSalida.dispositivo.id << " de tipo " << msgSalida.dispositivo.tipo << std::endl;
		Helper::output(stdout, ss, outputColour);
	}

	return 0;
}
