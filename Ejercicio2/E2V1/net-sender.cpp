#include <stddef.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>

#include "Config.h"
#include "Helper.h"
#include "includes.h"
#include "Queue.cpp"
#include "Socket.h"

int main() {
	std::stringstream ss;
	Config cfg("network.conf");
	std::string address = cfg.getString("broker address", "localhost");
	unsigned short port = (unsigned short) cfg.getInt("broker receiver port", 6112);
	Socket * connection;
	Queue<Net::iMessage> * q;
	Net::iMessage iMsg;
	Net::message msg;
	size_t bytes, expectedBytes = sizeof(msg);
	long connNumber;

	q = new Queue<Net::iMessage>(IPC::path, (int) IPC::QueueIdentifier::TO_BROKER_RECEIVER, "net-sender");
	q->get();

	connection = new Socket("net-sender");
	connection->active(address, port);
	connection->receive((char*) &connNumber, sizeof(connNumber));
	ss << connNumber;
	pid_t pid = fork();
	if (pid == 0) {
		execlp("./net-receiver", "net-receiver", ss.str().c_str(), NULL);
		perror("net-receiver - execlp: ");
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		perror("net-receiver - fork: ");
	}

	do {
		iMsg = q->receive((long) IPC::MessageTypes::ANY);
		msg.size = sizeof(iMsg);
		memcpy((void*) msg.message, (void*) &iMsg, msg.size);
		bytes = connection->send((char*) &msg, expectedBytes);
	} while (bytes == expectedBytes);

	Helper::output(stdout, "net-sender: connection ended");
	exit(EXIT_SUCCESS);

	return 0;
}

