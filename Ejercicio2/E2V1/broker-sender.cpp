#include <stddef.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#include "Config.h"
#include "Helper.h"
#include "includes.h"
#include "Queue.cpp"
#include "Socket.h"

int main() {
	std::string owner = "broker-sender";
	std::stringstream ss;

	Socket * master, *connection;
	pid_t pid;
	Queue<Broker::outgoingMessage> * fromBroker;

	Net::message msg;
	Net::interfaceMessage input;
	size_t bytes, sentBytes = sizeof(msg);
	long connectionNumber = 1;

	Config cfg("network.conf");
	unsigned short port = (unsigned short) cfg.getInt("broker sender port", 6113);

	fromBroker = new Queue<Broker::outgoingMessage>(IPC::path, (int) IPC::QueueIdentifier::TO_SENDER_FROM_BROKER, owner);
	fromBroker->get();

	master = new Socket(owner);
	if (master->passive(port) == -1) {
		Helper::output(stderr, "Error en open pasivo.\n");
		perror("broker-sender");
		exit(EXIT_FAILURE);
	}

	while (true) {
		// Espero que se conecte algun receiver
		connection = master->doAccept();
		if (connection == NULL) {
			Helper::output(stderr, "Error en accept.\n");
			exit(EXIT_FAILURE);
		}
		// Recibo el id de conexion, que indica que mensajes tengo que recibir de la cola.
		connection->receive((char*) &connectionNumber, sizeof(connectionNumber));
		pid = fork();
		if (pid < 0) {
			perror("fork: net-sender.");
		} else if (pid == 0) {
			do {
				input = fromBroker->receive(connectionNumber).interfaceMessage;
				msg.size = sizeof(input);
				memcpy((void*) msg.message, (void*) &input, msg.size);
				bytes = connection->send((char*) &msg, sentBytes);
				Helper::output(stdout, owner + " envie algo\n", Helper::Colours::BG_PURPLE);
			} while (bytes == sentBytes);
			Helper::output(stdout, "net-sender: connection ended");
			exit(EXIT_SUCCESS);
		}
	}
	return 0;
}

