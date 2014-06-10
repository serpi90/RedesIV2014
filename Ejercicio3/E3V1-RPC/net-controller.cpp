#include "includes.h"
#include "RPC/idManager.h"
#include "Queue.cpp"
#include "Config.h"
#include "Helper.h"

#include <list>
#include <stdlib.h>
#include <unistd.h>

int main() {
    Queue<struct iMessage> * fromInterface;
    Queue<struct iMessage> * toNet;
    Queue<struct iMessage> * toInterface;
    struct iMessage msg;

    std::list<long> hostList;
    std::list<long>::iterator i;
    pid_t pid;
    std::stringstream ss;
    std::string qid, remotePort;
    Config cfg("network.conf");

    CLIENT *clnt;
    getResult *rpc_get_result;
    queryResult *rpc_query_result;
    registerResult *rpc_register_result;

    std::string address = cfg.getString("id manager address", "localhost");
    clnt = clnt_create(address.c_str(), IDMANAGER, FIRST, "tcp");
    if (clnt == NULL) {
        clnt_pcreateerror(address.c_str());
        exit(1);
    }

    remotePort = cfg.getString("consumidor port", "6112");
    ss << Q_FROM_CTL_TO_NET;
    qid = ss.str();

    fromInterface = new Queue<struct iMessage>(PATH, Q_FROM_INTERFACE_TO_CTL, "net-sender");
    fromInterface->get();
    toNet = new Queue<struct iMessage>(PATH, Q_FROM_CTL_TO_NET, "net-sender");
    toNet->get();
    toInterface = new Queue<struct iMessage>(PATH, Q_FROM_NET_TO_INTERFACE, "net-sender");
    toInterface->get();

    while (true) {
        msg = fromInterface->receive(M_ANY);
        switch (msg.mtype) {
            case M_PROD:
                switch (msg.query.query) {
                    case REGISTER_PRODUCER:
                        rpc_register_result = register_producer_1((void*) NULL, clnt);
                        if (rpc_register_result == (registerResult *) NULL) {
                            clnt_perror(clnt, "register consummer call failed");
                        }
                        msg.query.id = rpc_register_result->registerResult_u.mtype;
                        toInterface->send(msg);
                        break;
                    case QUERY_CONSUMMERS:
                        rpc_query_result = query_consummers_1((void*) NULL, clnt);
                        if (rpc_query_result == (queryResult *) NULL) {
                            clnt_perror(clnt, "query consummer call failed");
                        }
                        msg.mtype = msg.query.id;
                        msg.query.id = M_PROD;
                        msg.query.consumidores = rpc_query_result->queryResult_u.mtypes;
                        toInterface->send(msg);
                        break;
                    default:
                        ss.str("");
                        ss << "Unexpected msg.query.type from M_PROD: " << msg.query.type << std::endl;
                        Helper::output(stderr, ss, RED);
                        break;
                }
                break;
            case M_CONS:
                switch (msg.query.query) {
                    case REGISTER_CONSUMMER:
                        rpc_register_result = register_consummer_1(&msg.query.type, clnt);
                        if (rpc_register_result == (registerResult *) NULL) {
                            clnt_perror(clnt, "register consummer call failed");
                        }
                        msg.query.id = rpc_register_result->registerResult_u.mtype;
                        toInterface->send(msg);
                        break;
                    default:
                        ss.str("");
                        ss << "Unexpected msg.query.type from M_CONS: " << msg.query.type << std::endl;
                        Helper::output(stderr, ss, RED);
                        break;
                }
                break;
            default:
                // TODO: guardar los pids, hacer get y comparar. Si es distinto matar.
                for (i = hostList.begin(); i != hostList.end() && *i != msg.mtype; i++);
                if (i == hostList.end()) {
                    rpc_get_result = get_1(&msg.mtype, clnt);
                    if (rpc_get_result == (getResult *) NULL) {
                        clnt_perror(clnt, "get call failed");
                    }

                    hostList.push_back(msg.mtype);
                    ss.str("");
                    ss << msg.mtype;
                    pid = fork();
                    if (pid < 0) {
                        perror("net-controller: fork()");
                        exit(EXIT_FAILURE);
                    } else if (pid == 0) {
                        execlp("./net-sender", "net-sender", rpc_get_result->getResult_u.address, remotePort.c_str(), qid.c_str(), ss.str().c_str(), NULL);
                        perror("net-sender- execlp().");
                        exit(EXIT_FAILURE);
                    }
                }
                toNet->send(msg);
                break;
        }
    }
    return 0;
}


