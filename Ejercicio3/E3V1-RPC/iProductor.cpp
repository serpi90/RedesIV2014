#include "iProductor.h"
#include "includes.h"
#include "Queue.cpp"
#include "Config.h"
#include <sstream>

iProductor::iProductor()
{
    std::stringstream ss;
    ss << "iProductor ";
    this->owner = ss.str();
    q = new Queue<struct msgAlmacen>(PATH, Q_FROM_INTERFACE_TO_CTL, owner);
    q->get();
    Config cfg("network.conf");

    std::string address = cfg.getString("id manager address", "localhost");

    clnt = clnt_create(address.c_str(), IDMANAGER, FIRST, "tcp");
    if (clnt == NULL)
    {
        clnt_pcreateerror(address.c_str());
        exit(1);
    }
}

void iProductor::producirOrden(struct orden orden)
{

    struct msgAlmacen msg;
    msg.orden = orden;
    queryResult *rpc_query_result;
    char * rpc_query_1_arg = NULL;

    rpc_query_result = query_1((void*) & rpc_query_1_arg, clnt);
    if (rpc_query_result == (queryResult *) NULL)
    {
        clnt_perror(clnt, "call failed");
    }

    for (unsigned i = 0; i < rpc_query_result->queryResult_u.mtype.mtype_len; i++)
    {
        msg.type = rpc_query_result->queryResult_u.mtype.mtype_val[i];
        if (msg.type)
        {
            q->send(msg);
        }
    }
}
