#include <arpa/inet.h>
#include <netinet/in.h>
#include <stddef.h>
#include <unistd.h>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>

#include "Config.h"
#include "Helper.h"
#include "includes.h"
#include "net-idManagerProtocol.h"
#include "Semaphore.h"
#include "Socket.h"

namespace IdManager {
	class IdManager {
		public:

			IdManager(std::string filename) {
				this->filename = filename;
				this->data = readFile(filename);
			}

			~IdManager() {
				clearData(&data);
			}

			long register_host(HostKind k, char * address) {
				this->data = readFile(filename);
				hostInfo host;
				strncpy(host.address, address, MAX_ADDRESS_LENGTH);
				host.mtype = data.nextMtype++;
				host.kind = k;
				this->addHost(&(data.hosts), host);
				this->writeFile(filename.c_str(), data);
				return host.mtype;
			}

		private:
			struct hostInfo {
					char address[MAX_ADDRESS_LENGTH];
					long mtype;
					HostKind kind;
			};

			struct hostArray {
					unsigned amount;
					struct hostInfo * hosts;
			};

			struct fileData {
					long nextMtype;
					struct hostArray hosts;
			};

			struct fileData data;
			std::string filename;

			void addHost(struct hostArray *array, struct hostInfo host) {
				if (array->hosts) {
					array->hosts = (IdManager::hostInfo*) realloc(array->hosts, (array->amount + 1) * sizeof(struct hostInfo));
				} else {
					array->hosts = (IdManager::hostInfo*) malloc(sizeof(struct hostInfo));
				}
				array->hosts[array->amount++] = host;
			}

			struct fileData readFile(std::string filename) {
				struct fileData data;
				FILE * file = fopen(filename.c_str(), "rb");
				if (file) {
					if (fread(&(data.nextMtype), sizeof(data.nextMtype), 1, file)) {
						fread(&(data.hosts.amount), sizeof(data.hosts.amount), 1, file);

						if (data.hosts.amount) {
							data.hosts.hosts = (IdManager::hostInfo*) malloc(sizeof(struct hostInfo) * data.hosts.amount);
							fread(data.hosts.hosts, sizeof(struct hostInfo), data.hosts.amount, file);
						} else {
							data.hosts.hosts = NULL;
						}
					} else {
						bzero(&data, sizeof(data));
						data.nextMtype = (long) IPC::MessageTypes::FIRST_AVAILABLE_MTYPE;
					}
					fclose(file);
				} else {
					bzero(&data, sizeof(data));
					data.nextMtype = (long) IPC::MessageTypes::FIRST_AVAILABLE_MTYPE;
				}
				return data;
			}

			bool writeFile(const char * filename, struct fileData data) {
				FILE * file = fopen(filename, "wb");
				if (file) {
					fwrite(&(data.nextMtype), sizeof(data.nextMtype), 1, file);
					fwrite(&(data.hosts.amount), sizeof(data.hosts.amount), 1, file);
					if (data.hosts.amount) {

						fwrite(data.hosts.hosts, sizeof(struct hostInfo), data.hosts.amount, file);
					}
					fclose(file);
				} else {
					return false;
				}
				return true;
			}

			void clearData(struct fileData* dataPtr) {
				dataPtr->nextMtype = (long) IPC::MessageTypes::FIRST_AVAILABLE_MTYPE;
				if (dataPtr->hosts.hosts) {
					free(dataPtr->hosts.hosts);
				}
				dataPtr->hosts.hosts = (struct hostInfo*) NULL;
				dataPtr->hosts.amount = 0;
			}
	};
}

int main() {
	Config cfg("network.conf");
	unsigned short port = (unsigned short) cfg.getInt("id manager port", 6111);
	Socket * master, *connection;
	pid_t pid;
	IdManager::message msg;
	size_t bytes, expectedBytes = sizeof(msg);
	char * address;
	std::stringstream ss;
	IdManager::IdManager * idm;

	// Prevent zombie processes.
	struct sigaction sigchld_action;
	sigchld_action.sa_handler = SIG_DFL;
	sigchld_action.sa_flags = SA_NOCLDWAIT;
	sigaction(SIGCHLD, &sigchld_action, NULL);

	idm = new IdManager::IdManager("ids.dat");

	master = new Socket("net-idManager");
	if (master->passive(port) == -1) {
		exit(EXIT_FAILURE);
	};
	Semaphore * mutex = new Semaphore(IPC::path, (int) IPC::SemaphoreIdentifier::MUTEX_ID_MANAGER, "idManager");
	mutex->get();

	while (true) {
		connection = master->doAccept();
		if (connection == NULL) {
			Helper::output(stderr, "Error en accept.\n");
			exit(EXIT_FAILURE);
		}
		pid = fork();
		if (pid < 0) {
			perror("fork: net-idManager.");
		} else if (pid == 0) {
			do {
				bytes = connection->receive((char*) &msg, expectedBytes);
				if (bytes == expectedBytes) {
					mutex->wait();
					switch (msg.type) {
						case IdManager::MessageType::REGISTER_HOST:
							address = inet_ntoa(connection->getRemoteAddress());
							msg.register_host.mtype = idm->register_host(msg.register_host.kind, address);
							mutex->post();
							ss.str("");
							ss << "idManager: REGISTER_HOST " << ": " << msg.register_host.mtype << std::endl;
							Helper::output(stdout, ss, Helper::Colours::GREEN);
							connection->send((char*) &msg, sizeof(msg));
							break;
						default:
							mutex->post();
							Helper::output(stderr, "ned-idManager: Wrong message received\n.", Helper::Colours::RED);
							exit(EXIT_FAILURE);
					}
				}
			} while (bytes == expectedBytes);
			Helper::output(stdout, "net-idManager: connection ended");
			exit(EXIT_SUCCESS);
		} else {
			connection->doClose();
		}
	}
	return 0;
}
