/*
 * broker.cpp
 *
 *  Created on: Jun 25, 2014
 *      Author: julian
 */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <sstream>
#include <string>
#include <utility>

#include "ArmarAnillo.h"
#include "Config.h"
#include "Helper.h"
#include "includes.h"
#include "net-idManagerProtocol.h"
#include "Queue.cpp"
#include "Semaphore.h"
#include "SharedMemory.cpp"
#include "Socket.h"

namespace Broker {
	class Broker {
		public:
			Broker(bool master) {
				owner = "broker";
				idReq = new Queue<IdManager::messageRequest>(IPC::path, (int) IPC::QueueIdentifier::ID_MANAGER_BROKER, owner);
				idReq->get();
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

				Config cfg("network.conf");
				std::string server = cfg.getString("id manager address", "localhost");
				unsigned short port = cfg.getInt("id manager port", 6111);
				toIdManager = new Socket(owner);
				if (toIdManager->active(server, port) == -1) {
					Helper::output(stdout, owner + " conexion con idManager fallo\n", Helper::Colours::RED);
					exit(EXIT_FAILURE);
				}

				IdManager::message idRequest;
				idRequest.register_host.kind = IdManager::HostKind::BROKER;
				idRequest.type = IdManager::MessageType::REGISTER_BROKER;
				toIdManager->send((char*) &idRequest, sizeof(idRequest));
				toIdManager->receive((char*) &idRequest, sizeof(idRequest));
				this->id = idRequest.register_host.mtype;

				mutexToken = new Semaphore(IPC::path, (int) IPC::SemaphoreIdentifier::MUTEX_TOKEN, owner);
				mutexToken->get();
				tengoToken = new Semaphore(IPC::path, (int) IPC::SemaphoreIdentifier::TENGO_TOKEN, owner);
				tengoToken->get();
				devuelvoToken = new Semaphore(IPC::path, (int) IPC::SemaphoreIdentifier::DEVUELVO_TOKEN, owner);
				devuelvoToken->get();

				shmToken = new SharedMemory<tokenShm>(IPC::path, (int) IPC::SharedMemoryIdentifier::BROKER_TOKEN, owner);
				shmToken->get();
				tokenData = shmToken->attach();

				pid_t pid = fork();
				if (pid == 0) {
					// establezcon el anillo
					unsigned short udp_port, mcast_port, tcp_port;
					std::string multicast_adddress;
					Config bcfg("broker.conf");
					udp_port = bcfg.getInt("udp port", 7777);
					tcp_port = bcfg.getInt("tcp port", 8888);
					mcast_port = bcfg.getInt("multicast port", 9999);
					multicast_adddress = bcfg.getString("multicast address", "255.0.0.99");

					ArmarAnillo * armador;
					armador = new ArmarAnillo(master, udp_port, multicast_adddress, mcast_port);
					armador->crearAnillo();
					armador->crearConexionesTCP(tcp_port, prevBroker, nextBroker);

					struct {
							long id;
							struct in_addr address;
					} myData, brokerData;
					std::string idCola, idBroker;
					std::stringstream ss;
					ss << (int) IPC::QueueIdentifier::TO_BROKER_FROM_BROKER;
					idCola = ss.str();
					ss.str("");
					pid_t pid;

					myData.id = this->id;
					myData.address = nextBroker->getLocalAddress();

					nextBroker->send((char*) &myData, sizeof(myData));
					do {
						prevBroker->receive((char*) &brokerData, sizeof(brokerData));
						if (myData.id == brokerData.id) {
							nextBroker->send((char*) &brokerData, sizeof(brokerData));
						}
						ss << brokerData.id;
						pid = fork();
						if (pid == 0) {
							execlp("./inter-broker-sender", "inter-broker-sender", inet_ntoa(brokerData.address), idCola.c_str(), ss.str().c_str(), NULL);
							perror("inter-broker-sender - execlp: ");
							exit(EXIT_FAILURE);
						} else if (pid < 0) {
							perror("inter-broker-sender - fork: ");
						}
						ss.str("");
					} while (myData.id != brokerData.id);
					// Lanzar token;
					token token;
					token.shm = *shmPlataforma;

					if (master) {
						nextBroker->send((char*) &token, sizeof(token));
					}
					while (true) {
						// Recibo token
						prevBroker->receive((char*) &token, sizeof(token));

						// TODO Zaraza token + colas

						mutexToken->wait();
						// Si lo necesitan l
						if (tokenData->necesitoToken > 0) {
							tokenData->token = token;
							mutexToken->post();

							tengoToken->post();
							devuelvoToken->wait();

							mutexToken->wait();
							token = tokenData->token;
							mutexToken->post();
						} else {
							mutexToken->post();
						}

						nextBroker->send((char*) &token, sizeof(token));
					}
					exit(EXIT_FAILURE);

				} else if (pid < 0) {
					perror("fork receptor token");
					exit(EXIT_FAILURE);
				}
			}

			void addNewId(long connectionNumber) {
				outgoingMessage response;
				IdManager::message message;
				IdManager::messageRequest request;
				request = idReq->receive((long) IPC::MessageTypes::ANY);
				message.type = IdManager::MessageType::REGISTER_HOST;
				message.register_host.kind = request.kind;
				toIdManager->send((char*) &message, sizeof(message));
				toIdManager->receive((char*) &message, sizeof(message));
				mtypeToConnection.insert(std::make_pair(message.register_host.mtype, connectionNumber));
				response.mtype = connectionNumber;
				response.interfaceMessage.type = Net::interfaceMessageType::ID_REPLY;
				response.interfaceMessage.id_reply.id = message.register_host.mtype;
				response.interfaceMessage.id_reply.mtype = request.mtype;
				toSender->send(response);
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
					// TODO creo que puedo sacar la exclusion, ya que espero sobre tengoToken
					mutex->wait();
					// Pido el token
					mutexToken->wait();
					tokenData->necesitoToken++;
					mutexToken->post();
					// Espero a que me den el token
					tengoToken->wait();
					// Me copio la shm del token
					mutexToken->wait();
					*shmPlataforma = tokenData->token.shm;
					mutexToken->post();

					// Envio la shm
					msg.interfaceMessage.syncMessage.mtype = id;
					msg.interfaceMessage.syncMessage.shm = *shmPlataforma;
					Helper::output(stdout, owner + " envio shm a la plataforma.\n", Helper::Colours::BG_BLUE);
					toSender->send(msg);

					// Espero la shm actualizada
					updated = syncPlat->receive(id);
					Helper::output(stdout, owner + " plataforma devolvio la shm.\n", Helper::Colours::BG_BLUE);
					*shmPlataforma = updated.shm;

					// copio la actualizada al token
					mutexToken->wait();
					tokenData->token.shm = *shmPlataforma;
					tokenData->necesitoToken--;
					mutexToken->post();
					// Aviso que se pueden llevar el token
					devuelvoToken->post();

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
			Queue<IdManager::messageRequest> * idReq;
			SharedMemory<tokenShm> * shmToken;
			Semaphore * tengoToken, *devuelvoToken, *mutexToken;
			tokenShm * tokenData;

			Semaphore * mutex;
			std::map<long, long> mtypeToConnection;
			std::string owner;
			ColaPlataforma::shared * shmPlataforma;
			Socket * toIdManager, *nextBroker, *prevBroker;
			long id;
	};
}

int main(int argc, char* argv[]) {
	Queue<Broker::message> * fromReceiver;
	Broker::message incoming;
	std::string owner = "broker";

	bool first = argc > 1 && strcmp("primero", argv[1]) == 0;

	fromReceiver = new Queue<Broker::message>(IPC::path, (int) IPC::QueueIdentifier::TO_BROKER_FROM_RECEIVER, owner);
	fromReceiver->get();
	std::stringstream ss;
	Broker::Broker broker(first);
	while (true) {
		incoming = fromReceiver->receive((long) IPC::MessageTypes::ANY);
		switch (incoming.request) {
			case Broker::Request::NEW_ID:
				ss << owner << " recibi NEW_ID " << incoming.mtype << " conn: " << incoming.connNumber << std::endl;
				Helper::output(stdout, ss, Helper::Colours::BG_YELLOW);
				broker.addNewId(incoming.connNumber);
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
