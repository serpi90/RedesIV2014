#include <stddef.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <set>

#include "Config.h"
#include "Helper.h"
#include "includes.h"
#include "Queue.cpp"
#include "Socket.h"

void notifyBrokerIfNecessary(Queue<Broker::message> * q, long cNr, Net::iMessage m) {
	static std::set<long> knownIds;
	long mtype = 0;
	Broker::message msg;
	switch (m.type) {
		case Net::iMessageType::ACTIVADO:
			mtype = m.activado.mtype;
			break;
		case Net::iMessageType::ARMADO:
			mtype = m.armado.mtype;
			break;
		case Net::iMessageType::DISPOSITIVO:
			mtype = m.dispositivo.mtype;
			break;
		case Net::iMessageType::BROKER_REQUEST:
			mtype = m.broker_request.mtype;
			break;
		case Net::iMessageType::SALIDA:
			mtype = m.salida.mtype;
			break;
		default:
			return;
	}
	if (mtype) {
		if (knownIds.find(mtype) == knownIds.end()) {
			knownIds.insert(mtype);
			msg.request = Broker::Request::NEW_ID;
			msg.mtype = mtype;
			msg.connNumber = cNr;
			q->send(msg);
		}
	}
}

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
	Net::message msg;
	Net::iMessage incomingMessage;
	Broker::message msgBrk;
	size_t bytes, expectedBytes = sizeof(incomingMessage);
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
		msgBrk.mtype = (long) IPC::MessageTypes::BROKER;
		msgBrk.request = Broker::Request::NEW_CONNECTION;
		msgBrk.connNumber = connectionNumber;
		toBroker->send(msgBrk);
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
					notifyBrokerIfNecessary(toBroker, connectionNumber, incomingMessage);
					// Envio el mensaje a la cola que coresponde.
					switch (incomingMessage.type) {
						case Net::iMessageType::ACTIVADO:
							activado->send(incomingMessage.activado);
							break;
						case Net::iMessageType::ARMADO:
							armado->send(incomingMessage.armado);
							break;
						case Net::iMessageType::DISPOSITIVO:
							dispositivo->send(incomingMessage.dispositivo);
							break;
						case Net::iMessageType::BROKER_REQUEST:
							toBroker->send(incomingMessage.broker_request);
							break;
						case Net::iMessageType::SALIDA:
							salida->send(incomingMessage.salida);
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

