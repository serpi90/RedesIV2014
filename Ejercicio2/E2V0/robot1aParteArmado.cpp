#include "includes.h"
#include "Queue.cpp"
#include "Helper.h"
#include "Config.h"
#include "iRobot1aParteArmado.h"
#include <unistd.h>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <string>

int main(int argc, char * argv[]) {
	if (argc < 2) {
		Helper::output(stderr, "Usage: robot1aParteArmado <id>\n", Helper::Colours::RED);
		exit(EXIT_FAILURE);
	}

	std::string owner;
	long me;

	Queue<ColaArmado::message> * colaDeArmado;
	ColaArmado::message msgArmado;
	iRobot1aParteArmado * interfaz;
	unsigned lugar;
	std::stringstream ss;
	const Helper::Colours outputColour = Helper::Colours::BLUE;
	Config cfg("config.conf");

	me = atol(argv[1]);
	owner = std::string("robot1aParteArmado ") + argv[1];

	srand(time(NULL) + me);

	colaDeArmado = new Queue<ColaArmado::message>(IPC::path, (long) IPC::QueueIdentifier::ARMADO, owner);
	colaDeArmado->get();
	interfaz = new iRobot1aParteArmado(me);

	while (true) {
		Helper::output(stdout, owner + " reservando.\n", outputColour);
		lugar = interfaz->reservar();
		ss << owner << " reserve lugar " << lugar << " esperando para armar." << std::endl;
		Helper::output(stdout, ss, outputColour);

		// TODO ver diagrama IPC, el receive esta arriba. de reservar y esperar.
		msgArmado = colaDeArmado->receive((long) IPC::MessageTypes::M_ANY);
		interfaz->esperarSiSacando();
		Helper::output(stdout, owner + " armando.\n", outputColour);

		ss << owner << " armando dispositivo " << msgArmado.dispositivo.id << " de tipo " << msgArmado.dispositivo.tipo << "." << std::endl;
		Helper::output(stdout, ss, outputColour);
		Helper::doSleep(1, cfg.getInt("r1a armado", 5));
		interfaz->colocarDispositivo(msgArmado.dispositivo, lugar);
		ss << owner << " coloque dispositivo " << msgArmado.dispositivo.id << " de tipo " << msgArmado.dispositivo.tipo << " en el lugar " << lugar << "." << std::endl;
		Helper::output(stdout, ss, outputColour);
		interfaz->avisarSiEsperandoParaSacar();
	}

	return 0;
}
