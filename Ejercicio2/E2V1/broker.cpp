/*
 * broker.cpp
 *
 *  Created on: Jun 25, 2014
 *      Author: julian
 */

#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>

#include "Helper.h"
#include "includes.h"
#include "Queue.cpp"

//#include "SharedMemory.cpp"

namespace Broker {
	class Broker {
		public:
			Broker() {
				owner = "broker";

				activado = new Queue<ColaActivado::message>(IPC::path, (int) IPC::QueueIdentifier::ACTIVADO_BROKER, owner);
				activado->get();
				armado = new Queue<ColaArmado::message>(IPC::path, (int) IPC::QueueIdentifier::ARMADO_BROKER, owner);
				armado->get();
				dispositivo = new Queue<ColaDispositivo::message>(IPC::path, (int) IPC::QueueIdentifier::DISPOSITIVOS_BROKER, owner);
				dispositivo->get();
				salida = new Queue<ColaSalida::message>(IPC::path, (int) IPC::QueueIdentifier::SALIDA_BROKER, owner);
				salida->get();
				toSender = new Queue<outgoingMessage>(IPC::path, (int) IPC::QueueIdentifier::TO_SENDER_FROM_BROKER, owner);
				toSender->get();
			}

			void addNewId(long id, long connectionNumber) {
				std::stringstream ss;
				ss << owner << " mapeando " << id << " a " << connectionNumber << std::endl;
				Helper::output(stdout, ss);
				mtypeToConnection.insert(std::make_pair(id, connectionNumber));
			}
			void avisameSiEstoyArmado(long id) {
				outgoingMessage msg;

				msg.mtype = mtypeToConnection.at(id);
				msg.interfaceMessage.destination = (long) IPC::MessageTypes::UNWRAPPER;
				msg.interfaceMessage.type = Net::interfaceMessageType::DISPOSITIVO;
				pid_t pid = fork();
				if (pid == 0) {
					msg.interfaceMessage.dispositivo = dispositivo->receive(id);
					msg.interfaceMessage.dispositivo.mtype = id;
					toSender->send(msg);
					Helper::output(stdout, owner + " avise a dispositivo que esta armado.\n", Helper::Colours::BG_BLUE);
					exit(EXIT_SUCCESS);

				} else if (pid < 0) {
					perror("fork: broker esperarDispositivoArmado.");
				}
			}
			void dameDispositivoParaArmar(long id) {
				outgoingMessage msg;
				msg.mtype = mtypeToConnection.at(id);
				msg.interfaceMessage.destination = (long) IPC::MessageTypes::UNWRAPPER;
				msg.interfaceMessage.type = Net::interfaceMessageType::ARMADO;
				pid_t pid = fork();
				if (pid == 0) {
					msg.interfaceMessage.armado = armado->receive((long) IPC::MessageTypes::ANY);
					msg.interfaceMessage.armado.mtype = id;
					std::stringstream ss;
					ss << "dispositivo: " << msg.interfaceMessage.armado.dispositivo.id << " tipo: " << msg.interfaceMessage.armado.dispositivo.tipo << std::endl;
					Helper::output(stdout, ss);
					toSender->send(msg);
					Helper::output(stdout, owner + " envie dispositivo para armar.\n", Helper::Colours::BG_BLUE);
					exit(EXIT_SUCCESS);
				} else if (pid < 0) {
					perror("fork: broker esperarDispositivoParaArmar.");
				}

			}
			void dameDispositivoParaSacarDeCintaSalida(long id, long type) {
				outgoingMessage msg;
				msg.mtype = mtypeToConnection.at(id);
				msg.interfaceMessage.destination = (long) IPC::MessageTypes::UNWRAPPER;
				msg.interfaceMessage.type = Net::interfaceMessageType::SALIDA;
				pid_t pid = fork();
				if (pid == 0) {
					msg.interfaceMessage.salida = salida->receive(type);
					msg.interfaceMessage.salida.mtype = type;
					toSender->send(msg);
					Helper::output(stdout, owner + " envie dispositivo listo.\n", Helper::Colours::BG_BLUE);
					exit(EXIT_SUCCESS);
				} else if (pid < 0) {
					perror("fork: broker sacarDispositivoDeCintaSalida.");
				}
			}
			void dameDispositivoParaSacarDePlataforma(long id) {
				outgoingMessage msg;
				msg.mtype = mtypeToConnection.at(id);
				msg.interfaceMessage.destination = (long) IPC::MessageTypes::UNWRAPPER;
				msg.interfaceMessage.type = Net::interfaceMessageType::ACTIVADO;
				pid_t pid = fork();
				if (pid == 0) {
					msg.interfaceMessage.activado = activado->receive((long) IPC::MessageTypes::ANY);
					msg.interfaceMessage.activado.mtype = id;
					toSender->send(msg);
					Helper::output(stdout, owner + " envie dispositivo activado para sacar de plataforma.\n", Helper::Colours::BG_BLUE);
					exit(EXIT_SUCCESS);
				} else if (pid < 0) {
					perror("fork: broker sacarDispositivoDePlataforma.");
				}

			}
		private:
			struct assocciation {
					long mtype;
					long connection;
			};

			Queue<ColaActivado::message> * activado;
			Queue<ColaArmado::message> * armado;
			Queue<ColaDispositivo::message> * dispositivo;
			Queue<ColaSalida::message> * salida;
			Queue<outgoingMessage> * toSender;
			std::map<long, long> mtypeToConnection;
			std::string owner;
	}
	;
}

int main() {
	Queue<Broker::message> * fromReceiver;
	Broker::message incoming;
	std::string owner = "broker";

	fromReceiver = new Queue<Broker::message>(IPC::path, (int) IPC::QueueIdentifier::TO_BROKER_FROM_RECEIVER, owner);
	fromReceiver->get();
	std::stringstream ss;
	Broker::Broker broker;
	while (true) {
		incoming = fromReceiver->receive((long) IPC::MessageTypes::ANY);
		switch (incoming.request) {
			case Broker::Request::NEW_ID:
				ss << owner << " recibi NEW_ID " << incoming.mtype << " conn: " << incoming.connNumber << std::endl;
				Helper::output(stdout, ss, Helper::Colours::BG_YELLOW);
				broker.addNewId(incoming.mtype, incoming.connNumber);
				break;
			case Broker::Request::AVISAME_SI_ESTOY_ARMADO:
				ss << owner << " recibi AVISAME_SI_ESTOY_ARMADO de " << incoming.mtype << std::endl;
				Helper::output(stdout, ss, Helper::Colours::BG_YELLOW);
				broker.avisameSiEstoyArmado(incoming.mtype);
				break;
			case Broker::Request::DAME_DISPOSITIVO_PARA_ARMAR:
				ss << owner << " recibi DAME_DISPOSITIVO_PARA_ARMAR de " << incoming.mtype << " de " << std::endl;
				Helper::output(stdout, ss, Helper::Colours::BG_YELLOW);
				broker.dameDispositivoParaArmar(incoming.mtype);
				break;
			case Broker::Request::DAME_DISPOSITIVO_PARA_SACAR_DE_CINTA_SALIDA:
				ss << owner << " recibi DAME_DISPOSITIVO_PARA_SACAR_DE_CINTA_SALIDA de " << incoming.mtype << " tipo " << incoming.type << std::endl;
				Helper::output(stdout, ss, Helper::Colours::BG_YELLOW);
				broker.dameDispositivoParaSacarDeCintaSalida(incoming.mtype, incoming.type);
				break;
			case Broker::Request::DAME_DISPOSITIVO_PARA_SACAR_DE_PLATAFORMA:
				ss << owner << " recibi DAME_DISPOSITIVO_PARA_SACAR_DE_PLATAFORMA de " << incoming.mtype << std::endl;
				Helper::output(stdout, ss, Helper::Colours::BG_YELLOW);
				broker.dameDispositivoParaSacarDePlataforma(incoming.mtype);
				break;
			default:
				ss << owner << " recibi algo que no es de " << incoming.mtype << " req: " << (long) incoming.request << " conn " << incoming.connNumber << std::endl;
				Helper::output(stdout, ss, Helper::Colours::BG_RED);
				break;
		}
	}
}
