#include "includes.h"
#include "Queue.cpp"
#include "Helper.h"
#include "Config.h"
#include "iRobot1aParteSacado.h"
#include <unistd.h>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <string>

int main(int argc, char * argv[]) {
	if (argc < 2) {
		Helper::output(stderr, "Usage: robot1aParteSacado <id>\n", Helper::Colours::RED);
		exit(EXIT_FAILURE);
	}

	std::string owner;
	long me;

	Queue<ColaSalida::message> * colaDeSalida;
	ColaSalida::message msgSalida;
	iRobot1aParteSacado* interfaz;
	struct dispositivo dispositivo;
	std::stringstream ss;
	const Helper::Colours outputColour = Helper::Colours::CYAN;
	Config cfg("config.conf");

	me = atol(argv[1]);
	owner = std::string("robot1aParteSacado ") + argv[1];

	srand(time(NULL) + me);

	colaDeSalida = new Queue<ColaSalida::message>(IPC::path, (long) IPC::QueueIdentifier::SALIDA, owner);
	colaDeSalida->get();
	interfaz = new iRobot1aParteSacado(me);

	while (true) {
		Helper::output(stdout, owner + " esperando un dispositivo para sacar.\n", outputColour);
		dispositivo = interfaz->esperarDispositivo();
		ss << owner << " tengo un dispositivo " << dispositivo.id << " esperando para ser sacado." << std::endl;
		Helper::output(stdout, ss, outputColour);

		interfaz->esperarSiArmando();
		ss << owner << " sacando dispositivo " << dispositivo.id << " de tipo " << dispositivo.tipo << "." << std::endl;
		Helper::output(stdout, ss, outputColour);

		// TODO ver diagrama IPC, el tomar esta arriba. de esperar.

		Helper::doSleep(1, cfg.getInt("r1b sacado", 5));
		dispositivo = interfaz->tomarDispositivo(dispositivo);
		ss << owner << " saque dispositivo " << dispositivo.id << " de tipo " << dispositivo.tipo << std::endl;
		Helper::output(stdout, ss, outputColour);
		msgSalida.mtype = dispositivo.tipo;
		msgSalida.dispositivo = dispositivo;
		Helper::doSleep(1, cfg.getInt("r1b colocado", 5));
		colaDeSalida->send(msgSalida);
		Helper::output(stdout, owner + " coloque el dispositivo en la cola de salida\n", outputColour);
		interfaz->avisarSiEsperandoParaArmar();
	}

	return 0;
}
