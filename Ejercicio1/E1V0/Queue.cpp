#ifndef QUEUE_CPP
#define QUEUE_CPP

#include "Queue.h"

Queue::Queue(std::string path, int identifier, std::string owner, bool exitOnFailure)
{
    this->owner = owner;
    this->identifier = identifier;
    this->exitOnFailure = exitOnFailure;

    key = ftok(path.c_str(), identifier);
    if (key == -1)
    {
        error("ftok");
    }
}

void Queue::create()
{
    qid = msgget(key, IPC_CREAT | IPC_EXCL | PERMISSIONS);
    if (qid == -1)
    {
        error("create");
    }
}

void Queue::get(void)
{
    qid = msgget(key, 0600);
    if (qid == -1)
    {
        error("get");
    }
}

void Queue::remove(void)
{
    if (msgctl(qid, IPC_RMID, NULL) == -1)
    {
        error("remove");
    }
}

void Queue::send(void * message, size_t size)
{
    if (msgsnd(qid, message, size - sizeof (long), 0) == -1)
    {
        error("send");
    }
}

void Queue::receive(void * message, size_t size, long type)
{
    if (msgrcv(qid, message, size - sizeof (long), type, 0) == -1)
    {
        error("receive");
    }
}

void Queue::error(std::string where)
{
    if (exitOnFailure)
    {
        std::stringstream ss;
        ss << owner << " \033[41m\033[30mError\33[0m qid=" << identifier << " (" << getpid() << ") QUEUE " << where;
        perror(ss.str().c_str());
        exit(EXIT_FAILURE);
    }
}

#endif
