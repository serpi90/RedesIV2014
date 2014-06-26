#include <cstdio>
#include <string>

#include "Helper.h"
#include "includes.h"
#include "Queue.cpp"

int main() {
	Queue<ColaArmado::message> * in;
	Queue<Net::interfaceMessage> * out;
	Net::interfaceMessage output;
	std::string owner = "net-wrapper-armado";

	in = new Queue<ColaArmado::message>(IPC::path, (int) IPC::QueueIdentifier::ARMADO_FROM_DISP_TO_CTL, owner);
	in->get();
	out = new Queue<Net::interfaceMessage>(IPC::path, (int) IPC::QueueIdentifier::FROM_CTL_TO_NET, owner);
	out->get();

	output.type = Net::interfaceMessageType::ARMADO;
	while (true) {
		output.armado = in->receive((long) IPC::MessageTypes::ANY);
		Helper::output(stdout, owner + " enviando.\n", Helper::Colours::PINK);
		out->send(output);
	}
	return 0;
}

