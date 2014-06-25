#include <stddef.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "Helper.h"
#include "includes.h"
#include "Queue.cpp"
#include "Socket.h"

int main(int argc, char** argv) {
	if (argc < 5) {
		Helper::output(stderr, "usage: net-sender <address> <port> <queue id> <mtype>\n", Helper::Colours::RED);
		exit(EXIT_FAILURE);
	}
	const char * address = argv[1];
	unsigned short port = (unsigned short) atoi(argv[2]);
	int qid = atoi(argv[3]);
	long mtype = atol(argv[4]);
	Socket * connection;
	Queue<Net::iMessage> * q;
	Net::iMessage iMsg;
	Net::message msg;
	size_t bytes, expectedBytes = sizeof(msg);

	connection = new Socket("net-sender");
	connection->active(address, port);
	q = new Queue<Net::iMessage>(IPC::path, qid, "net-sender");
	q->get();

	do {
		iMsg = q->receive(mtype);
		msg.size = sizeof(iMsg);
		memcpy((void*) msg.message, (void*) &iMsg, msg.size);
		bytes = connection->send((char*) &msg, expectedBytes);
	} while (bytes == expectedBytes);

	Helper::output(stdout, "net-sender: connection ended");
	exit(EXIT_SUCCESS);

	return 0;
}

