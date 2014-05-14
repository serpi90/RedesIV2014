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
#define PERMISSIONS 0644
#endif

template <typename T>
class Queue
{
public:

    /* Create an interface with kernel message queues.
     * Arguments:
     * path
     *          The to be used to identify the queue in the system.
     *          Must be an absolute path to a real file or folder
     * identifier
     *          Used to recognise different IPC structures in the same path
     * owner
     *          String used to otput errors when encountered (if exitOnFailure is true)
     * exitOnFailure
     *          optional parameter, defaults to true
     *          if true, when an error ocurs:
     *          - A message will be sent to stderr,
     *          - The program will terminate with exit(EXIT_FAILURE) */
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

    /* Create the messages queue.
     * Can only be used once in the system */
    void create()
    {
        qid = msgget(key, IPC_CREAT | IPC_EXCL | PERMISSIONS);
        if (qid == -1)
        {
            error("create");
        }
    }

    /* Get the messages queue.
     * Must have been created before.
     * Can be reused in the system */
    void get()
    {
        qid = msgget(key, 0600);
        if (qid == -1)
        {
            error("get");
        }
    }

    /* Remove the messages queue from the system.
     * Must have been created before.
     * Can only be used once in the system */
    void remove()
    {
        if (msgctl(qid, IPC_RMID, NULL) == -1)
        {
            error("remove");
        }
    }

    /* Send message to the queue */
    void send(T &message)
    {
        if (msgsnd(qid, (void*) &message, sizeof (T) - sizeof (long), 0) == -1)
        {
            error("send");
        }
    }

    /* Recive a message from the queue.
     * If mtype = 0, recive any message.
     * If mtype > 0, recive a message with that mtype.
     * Refer to man msgrcv for a detailed explanation of mtype < 0 */
    T receive(long mtype)
    {
        T message;
        if (msgrcv(qid, (void*) &message, sizeof (T) - sizeof (long), mtype, 0) == -1)
        {
            error("receive");
        }
        return message;
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