#include <cstdio>
#include <string>

#include "Helper.h"
#include "includes.h"
#include "Queue.cpp"

int main() {
	Queue<Broker::message> *in;
	Queue<Net::interfaceMessage> * out;
	Net::interfaceMessage imsg;
	std::string owner = "net-wrapper-broker";

	in = new Queue<Broker::message>(IPC::path, (int) IPC::QueueIdentifier::TO_BROKER, owner);
	in->get();
	out = new Queue<Net::interfaceMessage>(IPC::path, (int) IPC::QueueIdentifier::FROM_WRAPPER_TO_NET, owner);
	out->get();

	imsg.type = Net::interfaceMessageType::BROKER_REQUEST;
	while (true) {
		imsg.broker_request = in->receive((long) IPC::MessageTypes::ANY);
		Helper::output(stdout, owner + " enviando.\n", Helper::Colours::PINK);
		out->send(imsg);
	}
	return 0;
}

