#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <string>

#include "Config.h"
#include "Helper.h"
#include "includes.h"
#include "net-idManagerProtocol.h"
#include "Queue.cpp"

int main() {
	std::string owner = "dispositivo";
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

	Queue<IdManager::messageRequest> * toIdManager;
	IdManager::messageRequest idRequest;
	Queue<IdManager::messageReply> * fromIdManager;
	IdManager::messageReply idReply;

	toIdManager = new Queue<IdManager::messageRequest>(IPC::path, (int) IPC::QueueIdentifier::TO_ID_MANAGER, owner);
	toIdManager->get();
	fromIdManager = new Queue<IdManager::messageReply>(IPC::path, (int) IPC::QueueIdentifier::FROM_ID_MANAGER, owner);
	fromIdManager->get();

	idRequest.mtype = (long) IPC::MessageTypes::DISPOSITIVO;
	idRequest.kind = IdManager::HostKind::DISPOSITIVO;
	toIdManager->send(idRequest);
	idReply = fromIdManager->receive((long) IPC::MessageTypes::DISPOSITIVO);

	me.id = idReply.id;

	srand(time(NULL) + me.id);

	me.tipo = rand() % DISPOSITIVE_TYPES + 1;
	ss << "dispositivo " << me.id << " [" << me.tipo << "]";
	owner = ss.str();
	ss.str("");

	colaDeArmado = new Queue<ColaArmado::message>(IPC::path, (int) IPC::QueueIdentifier::ARMADO_FROM_DISP_TO_CTL, owner);
	colaDeArmado->get();
	colaDeDispositivos = new Queue<ColaDispositivo::message>(IPC::path, (int) IPC::QueueIdentifier::DISPOSITIVOS_FROM_CTL_TO_DISP, owner);
	colaDeDispositivos->get();
	colaDeActivado = new Queue<ColaActivado::message>(IPC::path, (int) IPC::QueueIdentifier::ACTIVADO_FROM_DISP_TO_CTL, owner);
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
