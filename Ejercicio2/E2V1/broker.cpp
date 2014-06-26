/*
 * broker.cpp
 *
 *  Created on: Jun 25, 2014
 *      Author: julian
 */

#include <map>

#include "includes.h"
//#include "net-idManagerProtocol.h"
#include "Queue.cpp"
//#include "SharedMemory.cpp"

namespace Broker {
	class Broker {
		public:
			Broker() {
				std::string owner = "broker";

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
				mtypeToConnection.insert(std::pair<long, long>(id, connectionNumber));
			}
			void esperarDispositivoArmado(long id) {
				outgoingMessage msg;
				pid_t pid = fork();
				if (pid == 0) {
					msg.mtype = mtypeToConnection.at(id);
					msg.message.type = Net::iMessageType::DISPOSITIVO;
					msg.message.dispositivo = dispositivo->receive(id);
					toSender->send(msg);
				} else if (pid < 0) {
					perror("fork: broker esperarDispositivoArmado.");
				}
			}
			void esperarDispositivoParaArmar(long id) {
				outgoingMessage msg;
				pid_t pid = fork();
				if (pid == 0) {
					msg.mtype = mtypeToConnection.at(id);
					msg.message.type = Net::iMessageType::ARMADO;
					msg.message.armado = armado->receive((long) IPC::MessageTypes::ANY);
					msg.message.armado.mtype = id;
					toSender->send(msg);
				} else if (pid < 0) {
					perror("fork: broker esperarDispositivoParaArmar.");
				}
			}
			void sacarDispositivoDeCintaSalida(long id, long type) {
				outgoingMessage msg;
				pid_t pid = fork();
				if (pid == 0) {
					msg.mtype = mtypeToConnection.at(id);
					msg.message.type = Net::iMessageType::SALIDA;
					msg.message.salida = salida->receive(type);
					msg.message.salida.mtype = type;
					toSender->send(msg);
				} else if (pid < 0) {
					perror("fork: broker sacarDispositivoDeCintaSalida.");
				}
			}
			void sacarDispositivoDePlataforma(long id) {
				outgoingMessage msg;
				pid_t pid = fork();
				if (pid == 0) {
					msg.mtype = mtypeToConnection.at(id);
					msg.message.type = Net::iMessageType::ACTIVADO;
					msg.message.activado = activado->receive(id);
					msg.message.activado.mtype = id;
					toSender->send(msg);
				} else if (pid < 0) {
					perror("fork: broker sacarDispositivoDePlataforma.");
				}
			}
		private:

			Queue<ColaActivado::message> * activado;
			Queue<ColaArmado::message> * armado;
			Queue<ColaDispositivo::message> * dispositivo;
			Queue<ColaSalida::message> * salida;
			Queue<outgoingMessage> * toSender;
			std::map<long, long> mtypeToConnection;
	};
}

int main() {
	Queue<Broker::message> * fromReceiver;
	Broker::message incoming;

	fromReceiver = new Queue<Broker::message>(IPC::path, (int) IPC::QueueIdentifier::TO_BROKER_FROM_RECEIVER, "broker");
	fromReceiver->get();

	Broker::Broker broker;
	while (true) {
		incoming = fromReceiver->receive((long) IPC::MessageTypes::ANY);
		switch (incoming.request) {
			case Broker::Request::NEW_CONNECTION:
				// TODO Es necesario esto?
				break;
			case Broker::Request::NEW_ID:
				broker.addNewId(incoming.mtype, incoming.connNumber);
				break;
			case Broker::Request::AVISAME_SI_ESTOY_ARMADO:
				broker.esperarDispositivoArmado(incoming.mtype);
				break;
			case Broker::Request::DAME_DISPOSITIVO_PARA_ARMAR:
				broker.esperarDispositivoParaArmar(incoming.mtype);
				break;
			case Broker::Request::DAME_DISPOSITIVO_PARA_SACAR_DE_CINTA_SALIDA:
				broker.sacarDispositivoDeCintaSalida(incoming.mtype, incoming.type);
				break;
			case Broker::Request::DAME_DISPOSITIVO_PARA_SACAR_DE_PLATAFORMA:
				broker.sacarDispositivoDePlataforma(incoming.mtype);
				break;
			default:
				break;
		}
	}
}
