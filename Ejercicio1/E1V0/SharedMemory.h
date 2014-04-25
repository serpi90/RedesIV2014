#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

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
#define PERMISSIONS 0753
#endif

class SharedMemory {
private:
    key_t key;
    int shmid;
    int identifier;
    std::string owner;
    unsigned int size;
    void * attachAddress;
    bool exitOnFailure;
    bool attached;

    void error(std::string);

public:
    SharedMemory(std::string, int, unsigned int, std::string owner, bool exit = true);
    ~SharedMemory();
    void create();
    void get();
    void remove();
    void * attach();
    void dettach();
};

#endif
