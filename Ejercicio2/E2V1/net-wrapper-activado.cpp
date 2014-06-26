#include <cstdio>
#include <string>

#include "Helper.h"
#include "includes.h"
#include "Queue.cpp"

int main() {
	Queue<ColaActivado::message> * in;
	Queue<Net::interfaceMessage> * out;
	Net::interfaceMessage imsg;
	std::string owner = "net-wrapper-activado";

	in = new Queue<ColaActivado::message>(IPC::path, (int) IPC::QueueIdentifier::ACTIVADO_FROM_DISP_TO_CTL, owner);
	in->get();
	out = new Queue<Net::interfaceMessage>(IPC::path, (int) IPC::QueueIdentifier::FROM_WRAPPER_TO_NET, owner);
	out->get();

	imsg.type = Net::interfaceMessageType::ACTIVADO;
	while (true) {
		imsg.activado = in->receive((long) IPC::MessageTypes::ANY);
		Helper::output(stdout, owner + " enviando.\n", Helper::Colours::PINK);
		out->send(imsg);
	}
	return 0;
}

