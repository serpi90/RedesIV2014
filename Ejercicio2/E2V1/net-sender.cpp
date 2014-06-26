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
	Queue<Net::interfaceMessage> * inputQueue;
	Net::interfaceMessage input;
	Net::message output;
	size_t bytes, sentBytes = sizeof(output);
	long connNumber;
	std::string owner = "net-sender";

	inputQueue = new Queue<Net::interfaceMessage>(IPC::path, (int) IPC::QueueIdentifier::FROM_CTL_TO_NET, owner);
	inputQueue->get();

	connection = new Socket(owner);
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
		input = inputQueue->receive((long) IPC::MessageTypes::ANY);
		output.size = sizeof(input);
		memcpy((void*) output.message, (void*) &input, output.size);
		bytes = connection->send((char*) &output, sentBytes);
		Helper::output(stdout, owner + " envie algo\n", Helper::Colours::D_RED);
	} while (bytes == sentBytes);

	Helper::output(stdout, "net-sender: connection ended");
	exit(EXIT_SUCCESS);

	return 0;
}

