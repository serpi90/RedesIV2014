#include <stddef.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#include "Config.h"
#include "Helper.h"
#include "includes.h"
#include "Queue.cpp"
#include "Socket.h"

int main(int argc, char** argv) {
	if (argc < 2) {
		Helper::output(stderr, "usage: net-receiver <connection number>\n", Helper::Colours::RED);
		exit(EXIT_FAILURE);
	}
	std::stringstream ss;
	std::string owner = "net-receiver";
	Config cfg("network.conf");
	std::string address = cfg.getString("broker address", "localhost");
	unsigned short port = (unsigned short) cfg.getInt("broker sender port", 6113);
	Socket * connection;
	Queue<Net::interfaceMessage> * out;
	Net::interfaceMessage output;
	Net::message msg;
	size_t bytes, expectedBytes = sizeof(msg);
	long connNumber = atol(argv[1]);

	out = new Queue<Net::interfaceMessage>(IPC::path, (int) IPC::QueueIdentifier::FROM_NET_TO_CTL, owner);
	out->get();

	connection = new Socket(owner);
	connection->active(address, port);
	connection->send((char*) &connNumber, sizeof(connNumber));

	do {
		bytes = connection->receive((char*) &msg, expectedBytes);
		Helper::output(stdout, owner + " recibi algo\n", Helper::Colours::D_CYAN);
		if (bytes == expectedBytes) {
			memcpy((void*) &output, (void*) msg.message, msg.size);
			output.destination = (long) IPC::MessageTypes::UNWRAPPER;
			out->send(output);
		}

	} while (bytes == expectedBytes);

	Helper::output(stdout, "net-receiver: connection ended");
	exit(EXIT_SUCCESS);

	return 0;
}

