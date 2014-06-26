#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <string>

#include "Config.h"
#include "Helper.h"
#include "includes.h"
#include "iRobot1aParteArmado.h"

int main(int argc, char * argv[]) {
	if (argc < 2) {
		Helper::output(stderr, "Usage: robot1aParteArmado <id>\n", Helper::Colours::RED);
		exit(EXIT_FAILURE);
		Helper::output(stderr, IPC::path);
	}

	std::string owner;
	long me;

	iRobot1aParteArmado * interfaz;
	struct dispositivo dispositivo;
	unsigned lugar;
	std::stringstream ss;
	const Helper::Colours outputColour = Helper::Colours::BLUE;
	Config cfg("config.conf");

	me = atol(argv[1]);
	owner = std::string("robot1aParteArmado ") + argv[1];

	srand(time(NULL) + me);

	interfaz = new iRobot1aParteArmado(me);

	while (true) {
		Helper::output(stdout, owner + " reservando.\n", outputColour);
		lugar = interfaz->reservar();
		ss << owner << " reserve lugar " << lugar << " esperando para armar." << std::endl;
		Helper::output(stdout, ss, outputColour);

		// TODO cambiar tomar a esperar.
		dispositivo = interfaz->esperarDispositivo();
		interfaz->esperarSiSacando();

		ss << owner << " armando dispositivo " << dispositivo.id << " de tipo " << dispositivo.tipo << "." << std::endl;
		Helper::output(stdout, ss, outputColour);
		Helper::doSleep(1, cfg.getInt("r1a armado", 5));
		interfaz->colocarDispositivo(dispositivo, lugar);
		ss << owner << " coloque dispositivo " << dispositivo.id << " de tipo " << dispositivo.tipo << " en el lugar " << lugar << "." << std::endl;
		Helper::output(stdout, ss, outputColour);
		interfaz->avisarSiEsperandoParaSacar();
	}

	return 0;
}
