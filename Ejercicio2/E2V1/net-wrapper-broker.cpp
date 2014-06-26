#include "includes.h"
#include "Queue.cpp"

int main() {
	Queue<Broker::message> *in;
	Queue<Net::iMessage> * out;
	Net::iMessage imsg;

	in = new Queue<Broker::message>(IPC::path, (int) IPC::QueueIdentifier::TO_BROKER_RECEIVER, "net-wrapper-broker");
	in->get();
	out = new Queue<Net::iMessage>(IPC::path, (int) IPC::QueueIdentifier::FROM_CTL_TO_NET, "net-wrapper-broker");
	out->get();

	imsg.type = Net::iMessageType::BROKER_REQUEST;
	while (true) {
		imsg.broker_request = in->receive((long) IPC::MessageTypes::ANY);
		out->send(imsg);
	}
	return 0;
}

