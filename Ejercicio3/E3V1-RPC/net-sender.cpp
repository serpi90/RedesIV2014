#include <stdlib.h>
#include <unistd.h>
#include "Helper.h"
#include "Socket.h"
#include "Config.h"
#include "Queue.cpp"

int main(int argc, char** argv) {
    if (argc < 5) {
        Helper::output(stderr, "usage: net-sender <address> <port> <queue id> <mtype>\n", RED);
        exit(EXIT_FAILURE);
    }
    const char * address = argv[1];
    unsigned short port = (unsigned short) atoi(argv[2]);
    int qid = atoi(argv[3]);
    long mtype = atol(argv[4]);
    Socket * connection;
    Queue<struct msgAlmacen> * q;
    struct msgAlmacen iMsg;
    struct netMessage msg;
    size_t bytes, expectedBytes = sizeof (msg);

    connection = new Socket("net-sender");
    connection->active(address, port);
    q = new Queue<struct msgAlmacen>(PATH, qid, "net-sender");
    q->get();

    do {
        iMsg = q->receive(mtype);
        msg.size = sizeof (iMsg);
        memcpy(msg.message, &iMsg, msg.size);
        bytes = connection->send((char*) &msg, expectedBytes);
    } while (bytes == expectedBytes);

    Helper::output(stdout, "net-sender: connection ended");
    exit(EXIT_SUCCESS);

    return 0;
}


