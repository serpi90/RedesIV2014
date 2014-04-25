#ifndef QUEUE_H
#define QUEUE_H

#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string>
#include <sstream>

#ifndef PERMISSIONS
#define PERMISSIONS 0753
#endif

class Queue {
private:
    key_t key;
    int qid;
    int identifier;
    std::string owner;
    bool exitOnFailure;

    void error(std::string);

public:
    Queue(std::string path, int identifier, std::string owner, bool exitOnFailure = true);
    void create();
    void get();
    void remove();
    void send(void * message, size_t size);
    void receive(void * message, size_t size, long type);
};

#endif
