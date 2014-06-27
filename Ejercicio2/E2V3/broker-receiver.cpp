#include <stddef.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#include "Config.h"
#include "Helper.h"
#include "includes.h"
#include "net-idManagerProtocol.h"
#include "Queue.cpp"
#include "Socket.h"

int main() {
	std::string owner = "broker-receiver";
	std::stringstream ss;

	Socket * master, *connection;
	pid_t pid;
	Queue<ColaActivado::message> * activado;
	Queue<ColaArmado::message> * armado;
	Queue<ColaDispositivo::message> * dispositivo;
	Queue<ColaSalida::message> * salida;
	Queue<Broker::message> * toBroker;
	Queue<ColaPlataforma::syncMessage> * syncPlat;
	Net::message msg;
	Net::interfaceMessage incomingMessage;
	Queue<IdManager::messageRequest> * idReq;
	Broker::message request;
	size_t bytes, expectedBytes = sizeof(msg);
	long connectionNumber = 1;

	Config cfg("network.conf");
	unsigned short port = (unsigned short) cfg.getInt("broker receiver port", 6112);

	activado = new Queue<ColaActivado::message>(IPC::path, (int) IPC::QueueIdentifier::ACTIVADO_BROKER, owner);
	activado->get();
	armado = new Queue<ColaArmado::message>(IPC::path, (int) IPC::QueueIdentifier::ARMADO_BROKER, owner);
	armado->get();
	dispositivo = new Queue<ColaDispositivo::message>(IPC::path, (int) IPC::QueueIdentifier::DISPOSITIVOS_BROKER, owner);
	dispositivo->get();
	salida = new Queue<ColaSalida::message>(IPC::path, (int) IPC::QueueIdentifier::SALIDA_BROKER, owner);
	salida->get();
	toBroker = new Queue<Broker::message>(IPC::path, (int) IPC::QueueIdentifier::TO_BROKER_FROM_RECEIVER, owner);
	toBroker->get();
	syncPlat = new Queue<ColaPlataforma::syncMessage>(IPC::path, (int) IPC::QueueIdentifier::PLATAFORMA_BROKER, owner);
	syncPlat->get();
	idReq = new Queue<IdManager::messageRequest>(IPC::path, (int) IPC::QueueIdentifier::ID_MANAGER_BROKER, owner);
	idReq->get();

	master = new Socket(owner);
	if (master->passive(port) == -1) {
		Helper::output(stderr, "Error en open pasivo.\n");
		perror("broker-receiver");
		exit(EXIT_FAILURE);
	}

	while (true) {
		// Espero que se conecte algun sender
		connection = master->doAccept();
		if (connection == NULL) {
			Helper::output(stderr, "Error en accept.\n");
			exit(EXIT_FAILURE);
		}
		pid = fork();
		if (pid < 0) {
			perror("fork: net-receiver.");
		} else if (pid == 0) {
			// Envio el id de conexion, para que el sender se lo pase al receiver. El receiver se lo pasa luego al broker-sender.
			connection->send((char*) &connectionNumber, sizeof(connectionNumber));
			do {
				bytes = connection->receive((char*) &msg, expectedBytes);
				if (bytes == expectedBytes) {
					memcpy((void*) &incomingMessage, (void*) msg.message, msg.size);
					// Envio el mensaje a la cola que coresponde.
					switch (incomingMessage.type) {
						case Net::interfaceMessageType::ACTIVADO:
							Helper::output(stdout, owner + " recibi ACTIVADO\n", Helper::Colours::BG_CYAN);
							activado->send(incomingMessage.activado);
							break;
						case Net::interfaceMessageType::ARMADO:
							Helper::output(stdout, owner + " recibi ARMADO\n", Helper::Colours::BG_CYAN);
							armado->send(incomingMessage.armado);
							break;
						case Net::interfaceMessageType::DISPOSITIVO:
							Helper::output(stdout, owner + " recibi DISPOSITIVO\n", Helper::Colours::BG_CYAN);
							dispositivo->send(incomingMessage.dispositivo);
							break;
						case Net::interfaceMessageType::BROKER_REQUEST:
							Helper::output(stdout, owner + " recibi BROKER_REQUEST\n", Helper::Colours::BG_CYAN);
							toBroker->send(incomingMessage.broker_request);
							break;
						case Net::interfaceMessageType::SALIDA:
							Helper::output(stdout, owner + " recibi SALIDA\n", Helper::Colours::BG_CYAN);
							salida->send(incomingMessage.salida);
							break;
						case Net::interfaceMessageType::PLATAFORMA_SYNC:
							Helper::output(stdout, owner + " recibi PLATAFORMA\n", Helper::Colours::BG_CYAN);
							syncPlat->send(incomingMessage.syncMessage);
							break;
						case Net::interfaceMessageType::ID_REQUEST:
							Helper::output(stdout, owner + " recibi ID_REQUEST\n", Helper::Colours::BG_CYAN);
							request.mtype = (long) IPC::MessageTypes::BROKER;
							request.request = Broker::Request::NEW_ID;
							request.connNumber = connectionNumber;
							idReq->send(incomingMessage.id_request);
							toBroker->send(request);
							break;
						default:
							Helper::output(stderr, owner + " mensaje no reconocido\n", Helper::Colours::RED);
					}
				}
			} while (bytes == expectedBytes);
			Helper::output(stdout, "net-receiver: connection ended");
			exit(EXIT_SUCCESS);
		} else {
			connectionNumber++;
		}
	}
	return 0;
}

