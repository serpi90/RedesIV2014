#ifndef SEMAPHOREARRAY_H
#define SEMAPHOREARRAY_H

#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string>
#include <sstream>

#ifndef PERMISSIONS
#define PERMISSIONS 0644
#endif

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
