#include <cstdio>
#include <string>

#include "Helper.h"
#include "includes.h"
#include "Queue.cpp"

int main() {
	Queue<Net::iMessage> * in;
	Queue<ColaActivado::message> * activado;
	Queue<ColaArmado::message> * armado;
	Queue<ColaDispositivo::message> * dispositivo;
	Queue<ColaSalida::message> * salida;
	Net::iMessage iMsg;
	std::string owner = "net-unwrapper";
	in = new Queue<Net::iMessage>(IPC::path, (int) IPC::QueueIdentifier::FROM_NET_TO_CTL, owner);
	in->get();
	activado = new Queue<ColaActivado::message>(IPC::path, (int) IPC::QueueIdentifier::ACTIVADO_FROM_CTL_TO_INTERFACE, owner);
	activado->get();
	armado = new Queue<ColaArmado::message>(IPC::path, (int) IPC::QueueIdentifier::ARMADO_FROM_CTL_TO_INTERFACE, owner);
	armado->get();
	dispositivo = new Queue<ColaDispositivo::message>(IPC::path, (int) IPC::QueueIdentifier::DISPOSITIVOS_FROM_CTL_TO_DISP, owner);
	dispositivo->get();
	salida = new Queue<ColaSalida::message>(IPC::path, (int) IPC::QueueIdentifier::SALIDA_FROM_CTL_TO_INTERFACE, owner);
	salida->get();
	while (true) {
		iMsg = in->receive((long) IPC::MessageTypes::ANY);
		switch (iMsg.type) {
			case Net::iMessageType::ACTIVADO:
				activado->send(iMsg.activado);
				break;
			case Net::iMessageType::ARMADO:
				armado->send(iMsg.armado);
				break;
			case Net::iMessageType::DISPOSITIVO:
				dispositivo->send(iMsg.dispositivo);
				break;
			case Net::iMessageType::SALIDA:
				salida->send(iMsg.salida);
				break;
			default:
				Helper::output(stderr, "Net-Unwrapper Recibi mensaje con tipo desconocido.\n", Helper::Colours::RED);
		}
	}
	return 0;
}

