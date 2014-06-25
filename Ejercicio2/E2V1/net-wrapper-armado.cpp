#include "includes.h"
#include "Queue.cpp"

int main() {
	Queue<ColaArmado::message> * in;
	Queue<Net::iMessage> * out;
	Net::iMessage imsg;

	in = new Queue<ColaArmado::message>(IPC::path, (int) IPC::QueueIdentifier::ARMADO_FROM_DISP_TO_CTL, "net-wrapper-armado");
	in->get();
	out = new Queue<Net::iMessage>(IPC::path, (int) IPC::QueueIdentifier::FROM_CTL_TO_NET, "net-wrapper-armado");
	out->get();

	imsg.type = Net::iMessageType::ARMADO;
	while (true) {
		imsg.armado = in->receive((long) IPC::MessageTypes::ANY);
		out->send(imsg);
	}
	return 0;
}

