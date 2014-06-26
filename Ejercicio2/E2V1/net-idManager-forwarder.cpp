#include <stddef.h>
#include <cstdio>
#include <cstdlib>
#include <string>

#include "Config.h"
#include "Helper.h"
#include "includes.h"
#include "net-idManagerProtocol.h"
#include "Queue.cpp"
#include "Socket.h"

int main() {
	Socket * connection;
	IdManager::message msg;
	IdManager::messageRequest request;
	IdManager::messageReply reply;
	Queue<IdManager::messageRequest> * in;
	Queue<IdManager::messageReply> * out;
	size_t bytes, expectedBytes = sizeof(msg);
	Config cfg("network.conf");
	std::string address = cfg.getString("id manager address", "localhost");
	unsigned short port = (unsigned short) cfg.getInt("id manager port", 6111);

	in = new Queue<IdManager::messageRequest>(IPC::path, (int) IPC::QueueIdentifier::TO_ID_MANAGER, "net-idManager-forwarder");
	in->get();
	out = new Queue<IdManager::messageReply>(IPC::path, (int) IPC::QueueIdentifier::FROM_ID_MANAGER, "net-idManager-forwarder");
	out->get();
	connection = new Socket("net-idManager-forwarder");
	connection->active(address, port);

	do {
		request = in->receive((long) IPC::MessageTypes::ANY);
		Helper::output(stdout, "net-idManager-forwarder: recibi REGISTER_HOST\n", Helper::Colours::BROWN);
		msg.type = IdManager::MessageType::REGISTER_HOST;
		msg.register_host.kind = request.kind;
		bytes = connection->send((char*) &msg, expectedBytes);
		if (bytes == expectedBytes) {
			connection->receive((char*) &msg, expectedBytes);
			reply.mtype = request.mtype;
			reply.id = msg.register_host.mtype;
			out->send(reply);
			Helper::output(stdout, "net-idManager-forwarder: recibi REGISTER_HOST\n", Helper::Colours::BROWN);
		}
	} while (bytes == expectedBytes);

	Helper::output(stdout, "net-idManager-forwarder: connection ended");
	exit(EXIT_SUCCESS);

	return 0;
}

