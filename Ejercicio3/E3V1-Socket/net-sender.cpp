#include <stdlib.h>
#include <unistd.h>
#include "Helper.h"
#include "Socket.h"
#include "Config.h"
#include "Queue.cpp"

int main(int argc, char** argv)
{
    if (argc < 4)
    {
        Helper::output(stderr, "usage: net-sender <address> <port> <queue id> <mtype>\n", RED);
        exit(EXIT_FAILURE);
    }
    const char * address = argv[0];
    unsigned short port = (unsigned short) atoi(argv[1]);
    int qid = atoi(argv[2]);
    long mtype = atol(argv[3]);
    Socket * connection;
    Queue<struct netMessage> * q;
    struct netMessage msg;
    size_t bytes, expectedBytes = sizeof (msg);

    connection = new Socket("net-sender");
    connection->active(address, port);
    q = new Queue<struct netMessage>(PATH, qid, "net-sender");
    q->get();

    do
    {
        msg = q->receive(mtype);
        bytes = connection->send((char*) &msg, expectedBytes);
    } while (bytes == expectedBytes);

    Helper::output(stdout, "net-sender: connection ended");
    exit(EXIT_SUCCESS);

    return 0;
}


