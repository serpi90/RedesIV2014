#include "includes.h"
#include "Queue.cpp"

int main() {
	Queue<ColaActivado::message> * in;
	Queue<Net::iMessage> * out;
	Net::iMessage imsg;

	in = new Queue<ColaActivado::message>(IPC::path, (int) IPC::QueueIdentifier::ACTIVADO_FROM_DISP_TO_CTL, "net-wrapper-activado");
	in->get();
	out = new Queue<Net::iMessage>(IPC::path, (int) IPC::QueueIdentifier::FROM_CTL_TO_NET, "net-wrapper-activado");
	out->get();

	imsg.type = Net::iMessageType::ACTIVADO;
	while (true) {
		imsg.activado = in->receive((long) IPC::MessageTypes::ANY);
		out->send(imsg);
	}
	return 0;
}

