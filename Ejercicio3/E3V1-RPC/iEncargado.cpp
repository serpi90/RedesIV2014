#include "iEncargado.h"
#include "includes.h"
#include "Queue.cpp"
#include "Config.h"
#include "Socket.h"
#include <sstream>
#include "RPC/idManager.h"

iEncargado::iEncargado(long numero)
{
    std::stringstream ss;
    this->numero = numero;
    ss << "iEncargado " << this->numero;

    Config cfg("network.conf");

    std::string address = cfg.getString("id manager address", "localhost");

    CLIENT *clnt;
    registerResult *result_1;
    char * register_1_arg;

    clnt = clnt_create(address.c_str(), IDMANAGER, FIRST, "tcp");
    if (clnt == NULL)
    {
        clnt_pcreateerror(address.c_str());
        exit(EXIT_FAILURE);
    }

    result_1 = register_1(&register_1_arg, clnt);
    if (result_1 == (registerResult *) NULL)
    {
        clnt_perror(clnt, "call failed");
    }


    this->id = result_1->registerResult_u.mtype;
    ss << " (" << this->id << ")";
    this->owner = ss.str();
    q = new Queue<struct msgAlmacen>(PATH, Q_FROM_NET_TO_INTERFACE, owner);
    q->get();
}

struct orden iEncargado::consumirOrden()
{
    struct msgAlmacen msg;
    msg = q->receive(this->id);
    return msg.orden;
}
