#include "includes.h"
#include "Queue.cpp"

int main() {
	Queue<ColaSalida::message> * in;
	Queue<Net::iMessage> * out;
	Net::iMessage imsg;

	in = new Queue<ColaSalida::message>(IPC::path, (int) IPC::QueueIdentifier::SALIDA_FROM_INTERFACE_TO_CTL, "net-wrapper-salida");
	in->get();
	out = new Queue<Net::iMessage>(IPC::path, (int) IPC::QueueIdentifier::FROM_CTL_TO_NET, "net-wrapper-salida");
	out->get();

	imsg.type = Net::iMessageType::SALIDA;
	while (true) {
		imsg.salida = in->receive((long) IPC::MessageTypes::ANY);
		out->send(imsg);
	}
	return 0;
}

