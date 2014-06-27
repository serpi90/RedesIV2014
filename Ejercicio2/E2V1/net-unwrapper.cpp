#include <cstdio>
#include <iostream>
#include <string>

#include "Helper.h"
#include "includes.h"
#include "net-idManagerProtocol.h"
#include "Queue.cpp"

int main() {
	Queue<Net::interfaceMessage> * in;
	Queue<ColaActivado::message> * activado;
	Queue<ColaArmado::message> * armado;
	Queue<ColaDispositivo::message> * dispositivo;
	Queue<ColaSalida::message> * salida;
	Queue<ColaPlataforma::syncMessage> * syncPlat;
	Queue<IdManager::messageReply> * idm;
	Net::interfaceMessage input;
	std::string owner = "net-unwrapper";
	std::stringstream ss;
	in = new Queue<Net::interfaceMessage>(IPC::path, (int) IPC::QueueIdentifier::FROM_NET_TO_UNWRAPPER, owner);
	in->get();
	activado = new Queue<ColaActivado::message>(IPC::path, (int) IPC::QueueIdentifier::ACTIVADO_FROM_UNRWAPPER_TO_INTERFACE, owner);
	activado->get();
	armado = new Queue<ColaArmado::message>(IPC::path, (int) IPC::QueueIdentifier::ARMADO_FROM_UNWRAPPER_TO_INTERFACE, owner);
	armado->get();
	dispositivo = new Queue<ColaDispositivo::message>(IPC::path, (int) IPC::QueueIdentifier::DISPOSITIVOS_FROM_UNWRAPPER_TO_DISP, owner);
	dispositivo->get();
	salida = new Queue<ColaSalida::message>(IPC::path, (int) IPC::QueueIdentifier::SALIDA_FROM_UNWRAPPER_TO_INTERFACE, owner);
	salida->get();
	syncPlat = new Queue<ColaPlataforma::syncMessage>(IPC::path, (int) IPC::QueueIdentifier::PLATAFORMA_FROM_UNWRAPPER, owner);
	syncPlat->get();
	idm = new Queue<IdManager::messageReply>(IPC::path, (int) IPC::QueueIdentifier::ID_MANAGER_FROM_UNWRAPPER_TO_INTERFACE, owner);
	idm->get();

	while (true) {
		input = in->receive((long) IPC::MessageTypes::ANY);
		switch (input.type) {
			case Net::interfaceMessageType::ACTIVADO:
				Helper::output(stdout, owner + " recibi activado.\n", Helper::Colours::PINK);
				activado->send(input.activado);
				break;
			case Net::interfaceMessageType::ARMADO:
				ss << owner << " recibi armado. dispositivo: " << input.armado.dispositivo.id << " [" << input.armado.dispositivo.tipo << "]" << std::endl;
				Helper::output(stdout, ss, Helper::Colours::PINK);
				armado->send(input.armado);
				break;
			case Net::interfaceMessageType::DISPOSITIVO:
				Helper::output(stdout, owner + " recibi dispositivo.\n", Helper::Colours::PINK);
				dispositivo->send(input.dispositivo);
				break;
			case Net::interfaceMessageType::SALIDA:
				Helper::output(stdout, owner + " recibi salida.\n", Helper::Colours::PINK);
				salida->send(input.salida);
				break;
			case Net::interfaceMessageType::PLATAFORMA_SYNC:
				Helper::output(stdout, owner + " recibi syncMessage.\n", Helper::Colours::PINK);
				syncPlat->send(input.syncMessage);
				break;
			case Net::interfaceMessageType::ID_REQUEST:
				Helper::output(stdout, owner + "ERROR: recibi idRequest.\n", Helper::Colours::RED);
				break;
			case Net::interfaceMessageType::ID_REPLY:
				Helper::output(stdout, owner + " recibi idReply.\n", Helper::Colours::PINK);
				idm->send(input.id_reply);
				break;
			default:
				Helper::output(stderr, "Net-Unwrapper Recibi mensaje con tipo desconocido.\n", Helper::Colours::RED);
		}
	}
	return 0;
}

