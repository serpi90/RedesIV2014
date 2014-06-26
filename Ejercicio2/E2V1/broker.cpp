/*
 * broker.cpp
 *
 *  Created on: Jun 25, 2014
 *      Author: julian
 */

#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <string>
#include <utility>

#include "Helper.h"
#include "includes.h"
#include "Queue.cpp"
#include "SharedMemory.cpp"
#include "Semaphore.h"

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
				syncPlat = new Queue<ColaPlataforma::syncMessage>(IPC::path, (int) IPC::QueueIdentifier::PLATAFORMA_BROKER, owner);
				syncPlat->get();
				mutex = new Semaphore(IPC::path, (int) IPC::SemaphoreIdentifier::MUTEX_BROKER_SYNC, owner);
				mutex->get();
				shm = new SharedMemory<ColaPlataforma::shared>(IPC::path, (int) IPC::SharedMemoryIdentifier::BROKER_PLAT, owner);
				shm->get();
				shmPlataforma = shm->attach();
				shmPlataforma->amount = 0;
				for (unsigned i = 0; i < ROBOT_AMOUNT; i++) {
					shmPlataforma->robotStatus[i] = ColaPlataforma::RobotStatus::NOT_WAITING;
				}
				for (unsigned i = 0; i < PLATFORM_CAPACITY; i++) {
					shmPlataforma->slot[i].status = ColaPlataforma::SlotStatus::FREE;
				}

			}

			void addNewId(long id, long connectionNumber) {
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
			void dameShm(long id) {
				// Recibi solicitud de shm
				outgoingMessage msg;
				ColaPlataforma::syncMessage updated;
				msg.mtype = mtypeToConnection.at(id);
				msg.interfaceMessage.destination = (long) IPC::MessageTypes::UNWRAPPER;
				msg.interfaceMessage.type = Net::interfaceMessageType::PLATAFORMA_SYNC;
				pid_t pid = fork();
				if (pid == 0) {
					// Atiendo 1 solicitud de shm por vez.
					mutex->wait();

					// Envio la shm
					msg.interfaceMessage.syncMessage.mtype = id;
					msg.interfaceMessage.syncMessage.shm = *shmPlataforma;
					Helper::output(stdout, owner + " envio shm a la plataforma.\n", Helper::Colours::BG_BLUE);
					toSender->send(msg);

					// Espero la shm actualizada
					updated = syncPlat->receive(id);
					Helper::output(stdout, owner + " plataforma devolvio la shm.\n", Helper::Colours::BG_BLUE);
					*shmPlataforma = updated.shm;
					std::stringstream ss;
					ss << "amount: " << shmPlataforma->amount;
					for (unsigned i = 0; i < ROBOT_AMOUNT; i++) {
						ss << " status " << i;
						switch (shmPlataforma->robotStatus[i]) {
							case ColaPlataforma::RobotStatus::NOT_WAITING:
								ss << " NOT_WAITING";
								break;
							case ColaPlataforma::RobotStatus::WAITING:
								ss << " WAITING";
								break;
						}
					}
					ss << std::endl;
					for (unsigned i = 0; i < PLATFORM_CAPACITY; i++) {
						ss << i << ": ";
						switch (shmPlataforma->slot[i].status) {
							case ColaPlataforma::SlotStatus::FREE:
								ss << "FREE ";
								break;
							case ColaPlataforma::SlotStatus::RESERVED:
								ss << "RSVD ";
								break;
							case ColaPlataforma::SlotStatus::OCCUPIED:
								ss << "OCPD ";
								break;
						}
					}
					ss << std::endl;
					Helper::output(stdout, ss);

					mutex->post();
					exit(EXIT_SUCCESS);
				} else if (pid < 0) {
					perror("fork: broker dameShm.");
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
			Queue<ColaPlataforma::syncMessage> * syncPlat;
			SharedMemory<ColaPlataforma::shared> * shm;

			Semaphore * mutex;
			std::map<long, long> mtypeToConnection;
			std::string owner;
			ColaPlataforma::shared * shmPlataforma;
	};
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
				ss << owner << " recibi DAME_DISPOSITIVO_PARA_ARMAR de " << incoming.mtype << std::endl;
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
			case Broker::Request::DAME_SHM:
				ss << owner << " recibi DAME_SHM de " << incoming.mtype << std::endl;
				Helper::output(stdout, ss, Helper::Colours::BG_YELLOW);
				broker.dameShm(incoming.mtype);
				break;
			default:
				ss << owner << " recibi algo que no es de " << incoming.mtype << " req: " << (long) incoming.request << " conn " << incoming.connNumber << std::endl;
				Helper::output(stdout, ss, Helper::Colours::BG_RED);
				break;
		}
	}
}
