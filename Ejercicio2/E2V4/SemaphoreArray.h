#ifndef SEMAPHOREARRAY_H
#define SEMAPHOREARRAY_H

#include <sys/ipc.h>
#include <string>

class SemaphoreArray {
	private:
		key_t key;
		int semid;
		int identifier;
		int amount;
		bool exitOnFailure;
		std::string owner;
		void error(std::string from);

	public:
		SemaphoreArray(std::string path, int identifier, int amount, std::string owner, bool exitOnFailure = true);
		void create();
		void get();
		void remove();
		void wait(int index);
		void post(int index);
};

#endif
