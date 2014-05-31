#include <stdlib.h>
#include <unistd.h>
#include <list>
#include "Helper.h"
#include "Socket.h"
#include "Config.h"
#include "Queue.cpp"
#include "RPC/idManager.h"

int main()
{
    Queue<struct msgAlmacen> * in;
    Queue<struct msgAlmacen> * out;
    struct msgAlmacen msg;

    std::list<long> hostList;
    std::list<long>::iterator i;
    pid_t pid;
    std::stringstream ss;
    std::string qid, remotePort;
    Config cfg("network.conf");
    std::string address = cfg.getString("id manager address", "localhost");

    // Prevent zombie processes.
    struct sigaction sigchld_action;
    sigchld_action.sa_handler = SIG_DFL;
    sigchld_action.sa_flags = SA_NOCLDWAIT;
    sigaction(SIGCHLD, &sigchld_action, NULL);

    CLIENT *clnt;
    getResult *rpc_get_result;

    clnt = clnt_create(address.c_str(), IDMANAGER, FIRST, "udp");
    if (clnt == NULL)
    {
        clnt_pcreateerror(address.c_str());
        exit(1);
    }

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

        for (i = hostList.begin(); i != hostList.end() && *i != msg.type; i++);
        if (i == hostList.end())
        {
            rpc_get_result = get_1(&msg.type, clnt);
            if (rpc_get_result == (getResult *) NULL)
            {
                clnt_perror(clnt, "call failed");
            }

            hostList.push_back(msg.type);
            ss.str("");
            ss << msg.type;
            pid = fork();
            if (pid < 0)
            {
                perror("net-controller: fork()");
                exit(EXIT_FAILURE);
            } else if (pid == 0)
            {
                execlp("./net-sender", "net-sender", rpc_get_result->getResult_u.address, remotePort.c_str(), qid.c_str(), ss.str().c_str(), NULL);
                perror("net-sender- execlp().");
                exit(EXIT_FAILURE);
            }
        }
        out->send(msg);
    }
    return 0;
}


