#include "includes.h"
#include "Queue.cpp"
#include "Semaphore.h"
#include "Config.h"
#include <sstream>

int main()
{
    Queue<struct iMessage> * qa;

    qa = new Queue<struct iMessage>(PATH, Q_FROM_INTERFACE_TO_CTL, "iniciador");
    qa->create();
    qa = new Queue<struct iMessage>(PATH, Q_FROM_CTL_TO_NET, "iniciador");
    qa->create();
    qa = new Queue<struct iMessage>(PATH, Q_FROM_NET_TO_INTERFACE, "iniciador");
    qa->create();

    Semaphore mutex(PATH, SEM_MUTEX_IDM, "iniciador");
    mutex.create();
    mutex.post();

    pid_t pid = fork();
    if (pid == 0)
    {
        execlp("./net-idManager", "net-idManager", NULL);
        perror("net-idManager - execlp: ");
        exit(EXIT_FAILURE);
    } else if (pid < 0)
    {
        perror("net-idManager - fork: ");
    }
    Config cfg("network.conf");
    std::string port = cfg.getString("consumidor port", "6112");
    std::stringstream ss;
    ss << Q_FROM_NET_TO_INTERFACE;
    pid = fork();
    if (pid == 0)
    {
        execlp("./net-receiver", "net-receiver", port.c_str(), ss.str().c_str(), NULL);
        perror("net-receiver - execlp: ");
        exit(EXIT_FAILURE);
    } else if (pid < 0)
    {
        perror("net-receiver - fork: ");
    }
}
