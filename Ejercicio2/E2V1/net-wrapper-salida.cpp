#include <cstdio>
#include <string>

#include "Helper.h"
#include "includes.h"
#include "Queue.cpp"

int main() {
	Queue<ColaSalida::message> * in;
	Queue<Net::interfaceMessage> * out;
	Net::interfaceMessage imsg;
	std::string owner = "net-wrapper-salida";

	in = new Queue<ColaSalida::message>(IPC::path, (int) IPC::QueueIdentifier::SALIDA_FROM_INTERFACE_TO_CTL, owner);
	in->get();
	out = new Queue<Net::interfaceMessage>(IPC::path, (int) IPC::QueueIdentifier::FROM_WRAPPER_TO_NET, owner);
	out->get();

	imsg.type = Net::interfaceMessageType::SALIDA;
	while (true) {
		imsg.salida = in->receive((long) IPC::MessageTypes::ANY);
		Helper::output(stdout, owner + " enviando.\n", Helper::Colours::PINK);
		out->send(imsg);
	}
	return 0;
}

