#include "includes.h"
#include "Queue.cpp"
#include "Helper.h"
#include "Config.h"
#include <unistd.h>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <string>

int main(int argc, char * argv[]) {
	if (argc < 2) {
		Helper::output(stderr, "Usage: dispositivo <id>\n", Helper::Colours::RED);
		exit(EXIT_FAILURE);
	}

	std::string owner;
	std::stringstream ss;
	struct dispositivo me;
	const Helper::Colours outputColour = Helper::Colours::PURPLE;

	Queue<ColaArmado::message> * colaDeArmado;
	ColaArmado::message msgArmado;
	Queue<ColaDispositivo::message> * colaDeDispositivos;
	ColaDispositivo::message msgDispositivo;
	Queue<ColaActivado::message> * colaDeActivado;
	ColaActivado::message msgActivado;
	Config cfg("config.conf");

	me.id = atol(argv[1]);

	srand(time(NULL) + me.id);

	me.tipo = rand() % DISPOSITIVE_TYPES + 1;
	ss << "dispositivo " << me.id << " [" << me.tipo << "]";
	owner = ss.str();
	ss.str("");

	colaDeArmado = new Queue<ColaArmado::message>(IPC::path, (long) IPC::QueueIdentifier::ARMADO, owner);
	colaDeArmado->get();
	colaDeDispositivos = new Queue<ColaDispositivo::message>(IPC::path, (long) IPC::QueueIdentifier::DISPOSITIVOS, owner);
	colaDeDispositivos->get();
	colaDeActivado = new Queue<ColaActivado::message>(IPC::path, (long) IPC::QueueIdentifier::ACTIVADO, owner);
	colaDeActivado->get();

	msgArmado.mtype = me.id;
	msgArmado.dispositivo = me;

	Helper::output(stdout, owner + " poniendome en lista para ser armado.\n", outputColour);
	colaDeArmado->send(msgArmado);

	msgDispositivo = colaDeDispositivos->receive(me.id);
	Helper::output(stdout, owner + " estoy armado.\n", outputColour);

	Helper::doSleep(1, cfg.getInt("dispositivo max activado", 5));

	msgActivado.mtype = me.id;
	msgActivado.dispositivo = me;
	colaDeActivado->send(msgActivado);
	Helper::output(stdout, owner + " estoy activado.\n", outputColour);
	return 0;
}
