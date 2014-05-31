#include <stdlib.h>
#include <unistd.h>
#include <list>
#include "Helper.h"
#include "Socket.h"
#include "Config.h"
#include "Queue.cpp"
#include "net-idManagerProtocol.h"

int main()
{
    Queue<struct msgAlmacen> * in;
    Queue<struct msgAlmacen> * out;
    struct msgAlmacen msg;

    Socket * connection;
    struct idManagerMessage queryMsg;
    std::list<struct hostInfo> hostList;
    std::list<struct hostInfo>::iterator i;
    pid_t pid;
    std::stringstream ss;
    std::string qid, remotePort;
    Config cfg("network.conf");
    struct hostInfo host;
    unsigned short port = (unsigned short) cfg.getInt("id manager port", 6111);
    std::string address = cfg.getString("id manager address", "localhost");

    connection = new Socket("net-controller");
    connection->active(address, port);
    remotePort = cfg.getString("consumidor port", "6112");
    ss << Q_FROM_CTL_TO_NET;
    qid = ss.str();

    in = new Queue<struct msgAlmacen>(PATH, Q_FROM_INTERFACE_TO_CTL, "net-sender");
    in->get();
    out = new Queue<struct msgAlmacen>(PATH, Q_FROM_CTL_TO_NET, "net-sender");
    out->get();

    while (true)
    {
        msg = in->receive(0);

        for (i = hostList.begin(); i != hostList.end() && i->mtype != msg.type; i++);
        if (i == hostList.end())
        {
            queryMsg.type = GET;
            connection->send((char*) &queryMsg, sizeof (queryMsg));
            connection->receive((char*) &queryMsg, sizeof (queryMsg));
            ss.str("");
            host = queryMsg.response.info;
            ss << host.mtype;
            pid = fork();
            if (pid < 0)
            {
                perror("net-controller: fork()");
                exit(EXIT_FAILURE);
            } else if (pid == 0)
            {
                execlp("./net-sender", "net-sender", host.address, remotePort.c_str(), qid.c_str(), ss.str().c_str(), NULL);
                perror("net-sender- execlp().");
                exit(EXIT_FAILURE);
            }
        }
        out->send(msg);
    }
    return 0;
}


