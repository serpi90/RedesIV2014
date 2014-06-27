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

int main(int argc, char** argv) { //addres(string) nombre(int) id(long))
	if (argc < 4) {
		perror("usage: address nombreCola idCola\n");
		exit(EXIT_FAILURE);
	}
	std::string address = argv[1];
	int nombreCola = argv[2];
	long id = argv[3];

	std::stringstream ss;
	Config cfg("network.conf");
	//std::string address = cfg.getString("broker address", "localhost");
	unsigned short port = (unsigned short) cfg.getInt("broker receiver port", 6112);
	Socket * connection;
	Queue<Net::interfaceMessage> * inputQueue;
	Net::interfaceMessage input;
	Net::message output;
	size_t bytes, sentBytes = sizeof(output);
	long connNumber;
	std::string owner = "net-sender";

	inputQueue = new Queue<Net::interfaceMessage>(IPC::path, nombreCola, owner);
	inputQueue->get();

	connection = new Socket(owner);
	connection->active(address, port);
	connection->receive((char*) &connNumber, sizeof(connNumber));
	ss << connNumber;
	pid_t pid = fork();

	do {
		input = inputQueue->receive(id);
		output.size = sizeof(input);
		memcpy((void*) output.message, (void*) &input, output.size);
		bytes = connection->send((char*) &output, sentBytes);
		//Helper::output(stdout, owner + " envie algo\n", Helper::Colours::D_RED);
	} while (bytes == sentBytes);

	Helper::output(stdout, "net-sender: connection ended");
	exit(EXIT_SUCCESS);

	return 0;
}

