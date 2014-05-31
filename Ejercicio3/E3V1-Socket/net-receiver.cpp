#include <stdlib.h>
#include <unistd.h>
#include "Helper.h"
#include "Socket.h"
#include "Config.h"
#include "Queue.cpp"

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        Helper::output(stderr, "usage: net-receiver <port> <queue id>\n", RED);
        exit(EXIT_FAILURE);
    }
    unsigned short port = (unsigned short) atoi(argv[0]);
    int qid = atoi(argv[1]);
    Socket * master, * connection;
    pid_t pid;
    Queue<struct netMessage> * q;
    struct netMessage msg;
    size_t bytes, expectedBytes = sizeof (msg);

    q = new Queue<struct netMessage>(PATH, qid, "net-receiver");
    q->get();
    master = new Socket("net-receiver");
    master->passive(port);

    while (true)
    {
        connection = master->doAccept();
        if (connection == NULL)
        {
            Helper::output(stderr, "Error en accept.\n");
            exit(EXIT_FAILURE);
        }
        pid = fork();
        if (pid <= 0)
        {
            perror("fork: net-receiver.");
        } else if (pid == 0)
        {
            do
            {
                bytes = connection->receive((char*) &msg, expectedBytes);
                if (bytes == expectedBytes)
                {
                    q->send(msg);
                }
            } while (bytes == expectedBytes);
            Helper::output(stdout, "net-receiver: connection ended");
            exit(EXIT_SUCCESS);
        }
    }
    return 0;
}

