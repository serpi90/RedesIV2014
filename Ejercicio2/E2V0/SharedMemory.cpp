#ifndef SHAREDMEMORY_CPP
#define SHAREDMEMORY_CPP

#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string>
#include <sstream>

#ifndef PERMISSIONS
#define PERMISSIONS 0644
#endif

template<typename S>
class SharedMemory {
	public:

		SharedMemory<S>(std::string path, unsigned int identifier, std::string owner, bool exit = true) {
			this->owner = owner;
			this->attached = false;
			this->size = sizeof(S);
			this->identifier = identifier;
			this->exitOnFailure = exit;

			key = ftok(path.c_str(), identifier);
			if (key == -1) {
				error("ftok");
			}
		}

		~SharedMemory() {
			dettach();
		}

		void create() {
			shmid = shmget(key, size, IPC_CREAT | IPC_EXCL | PERMISSIONS);
			if (shmid == -1) {
				error("create");
			}
		}

		void get() {
			shmid = shmget(key, size, PERMISSIONS);
			if (shmid == -1) {
				error("get");
			}
		}

		void remove() {
			if (shmctl(shmid, IPC_RMID, NULL) == -1) {
				error("remove");
			}
		}

		S * attach() {
			if (!attached) {
				attachAddress = (S *) shmat(shmid, NULL, PERMISSIONS);
				if (attachAddress == (void *) -1) {
					error("attach");
				}
				attached = true;
			}
			return attachAddress;
		}

		void dettach() {
			if (attached) {
				if (shmdt((void *) attachAddress) == -1) {
					error("dettach");
				}
				attached = false;
			}

		}
	private:
		key_t key;
		int shmid;
		int identifier;
		std::string owner;
		unsigned int size;
		S * attachAddress;
		bool exitOnFailure;
		bool attached;

		void error(std::string where) {
			if (exitOnFailure) {
				std::stringstream ss;
				ss << owner << " \033[41m\033[30mError\33[0m shmid=" << identifier << " (" << getpid() << ") SHM " << where;
				perror(ss.str().c_str());
				exit(EXIT_FAILURE);
			}
		}
};

#endif
