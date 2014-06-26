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
	Config cfg("network.conf");
	std::string address = cfg.getString("broker address", "localhost");
	unsigned short port = (unsigned short) cfg.getInt("broker sender port", 6113);
	Socket * connection;
	Queue<Net::iMessage> * q;
	Net::iMessage iMsg;
	Net::message msg;
	size_t bytes, expectedBytes = sizeof(msg);
	long connNumber = atol(argv[1]);

	q = new Queue<Net::iMessage>(IPC::path, (int) IPC::QueueIdentifier::TO_BROKER_RECEIVER, "net-receiver");
	q->get();

	connection = new Socket("net-receiver");
	connection->active(address, port);
	connection->send((char*) &connNumber, sizeof(connNumber));

	do {
		bytes = connection->receive((char*) &msg, expectedBytes);
		if (bytes == expectedBytes) {
			memcpy((void*) &iMsg, (void*) msg.message, msg.size);
			q->send(iMsg);
		}

	} while (bytes == expectedBytes);

	Helper::output(stdout, "net-receiver: connection ended");
	exit(EXIT_SUCCESS);

	return 0;
}

