#include "includes.h"
#include "Queue.cpp"

int main() {
	Queue<ColaDispositivo::message> * in;
	Queue<Net::iMessage> * out;
	Net::iMessage imsg;

	in = new Queue<ColaDispositivo::message>(IPC::path, (int) IPC::QueueIdentifier::DISPOSITIVOS_FROM_PLATAFORMA_TO_CTL, "net-wrapper-dispositivo");
	in->get();
	out = new Queue<Net::iMessage>(IPC::path, (int) IPC::QueueIdentifier::FROM_CTL_TO_NET, "net-wrapper-dispositivo");
	out->get();

	imsg.type = Net::iMessageType::DISPOSITIVO;
	while (true) {
		imsg.dispositivo = in->receive((long) IPC::MessageTypes::M_ANY);
		out->send(imsg);
	}
	return 0;
}

