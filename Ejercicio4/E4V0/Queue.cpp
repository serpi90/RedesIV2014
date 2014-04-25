#ifndef QUEUE_CPP
#define QUEUE_CPP

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

template <typename T>
class Queue {
public:

    Queue<T>(std::string path, int identifier, std::string owner, bool exitOnFailure = true)
    {
        this->owner = owner;
        this->identifier = identifier;
        this->exitOnFailure = exitOnFailure;
        this->qid = 0;
        key = ftok(path.c_str(), identifier);
        if (key == -1)
        {
            error("ftok");
        }
    }

    void create()
    {
        qid = msgget(key, IPC_CREAT | IPC_EXCL | PERMISSIONS);
        if (qid == -1)
        {
            error("create");
        }
    }

    void get()
    {
        qid = msgget(key, 0600);
        if (qid == -1)
        {
            error("get");
        }
    }

    void remove()
    {
        if (msgctl(qid, IPC_RMID, NULL) == -1)
        {
            error("remove");
        }
    }

    void send(T * message)
    {
        if (msgsnd(qid, message, sizeof (T) - sizeof (long), 0) == -1)
        {
            error("send");
        }
    }

    void receive(T * message, long type)
    {
        if (msgrcv(qid, message, sizeof (T) - sizeof (long), type, 0) == -1)
        {
            error("receive");
        }
    }
private:
    key_t key;
    int qid;
    int identifier;
    std::string owner;
    bool exitOnFailure;

    void error(std::string where)
    {
        if (exitOnFailure)
        {
            std::stringstream ss;
            ss << owner << " \033[41m\033[30mError\33[0m qid=" << identifier << " (" << getpid() << ") QUEUE " << where;
            perror(ss.str().c_str());
            exit(EXIT_FAILURE);
        }
    }
};

#endif