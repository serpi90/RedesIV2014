#include <cstdio>
#include <string>

#include "Helper.h"
#include "includes.h"
#include "Queue.cpp"

int main() {
	Queue<ColaDispositivo::message> * in;
	Queue<Net::interfaceMessage> * out;
	Net::interfaceMessage imsg;
	std::string owner = "net-wrapper-dispositivo";

	in = new Queue<ColaDispositivo::message>(IPC::path, (int) IPC::QueueIdentifier::DISPOSITIVOS_FROM_PLATAFORMA_TO_CTL, owner);
	in->get();
	out = new Queue<Net::interfaceMessage>(IPC::path, (int) IPC::QueueIdentifier::FROM_WRAPPER_TO_NET, owner);
	out->get();

	imsg.type = Net::interfaceMessageType::DISPOSITIVO;
	while (true) {
		imsg.dispositivo = in->receive((long) IPC::MessageTypes::ANY);
		Helper::output(stdout, owner + " enviando.\n", Helper::Colours::PINK);
		out->send(imsg);
	}
	return 0;
}

