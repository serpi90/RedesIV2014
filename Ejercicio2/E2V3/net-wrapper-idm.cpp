#include <cstdio>
#include <string>

#include "Helper.h"
#include "includes.h"
#include "net-idManagerProtocol.h"
#include "Queue.cpp"

int main() {
	Net::interfaceMessage imsg;
	Queue<IdManager::messageRequest> * in;
	Queue<Net::interfaceMessage> * out;
	std::string owner = "net-idManager-forwarder";

	in = new Queue<IdManager::messageRequest>(IPC::path, (int) IPC::QueueIdentifier::ID_MANAGER_FROM_INTERFACE_TO_WRAPPER, owner);
	in->get();
	out = new Queue<Net::interfaceMessage>(IPC::path, (int) IPC::QueueIdentifier::FROM_WRAPPER_TO_NET, owner);
	out->get();

	imsg.type = Net::interfaceMessageType::ID_REQUEST;
	while (true) {
		imsg.id_request = in->receive((long) IPC::MessageTypes::ANY);
		Helper::output(stdout, owner + " enviando.\n", Helper::Colours::PINK);
		out->send(imsg);
	}
	return 0;
}

