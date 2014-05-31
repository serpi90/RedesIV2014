#include <cstring>
#include "Socket.h"
#include "Semaphore.h"
#include "net-idManagerProtocol.h"
#include "Config.h"
#include "Helper.h"
#include "includes.h"

class IdManager
{
public:

    IdManager(std::string filename)
    {
        this->filename = filename;

    }

    struct hostInfo get(long mtype)
    {
        struct hostInfo host;
        FILE * file = fopen(filename.c_str(), "rb");
        do
        {
            fread(&host, sizeof (host), 1, file);
        } while (!feof(file) && host.mtype != mtype);
        if (feof(file))
        {
            host.mtype = 0;
        }
        fclose(file);
        return host;
    }

    long doRegister(char * address)
    {
        long mtype = 0;
        struct hostInfo host;
        FILE * file;
        file = fopen(filename.c_str(), "rb");
        do
        {
            fread(&host, sizeof (host), 1, file);
            mtype++;
        } while (!feof(file) && host.mtype != mtype + 1);
        fclose(file);
        file = fopen(filename.c_str(), "ab");
        strncpy(host.address, address, MAX_ADDRESS_LENGTH);
        host.mtype = mtype;
        fwrite(&host, sizeof (host), 1, file);
        fclose(file);
        return mtype;

    }

    struct hostInfo * query()
    {
        struct hostInfo * list = NULL, host;
        unsigned length = 0, current = 0;
        ;
        FILE * file = fopen(filename.c_str(), "rb");
        do
        {
            fread(&host, sizeof (host), 1, file);
            if (!feof(file))
            {
                if (current >= length)
                {
                    if (length == 0)
                    {
                        length = 1;
                        list = (struct hostInfo*) malloc(sizeof (struct hostInfo));
                    } else
                    {
                        length *= 2;
                        list = (struct hostInfo*) realloc(list, sizeof (struct hostInfo) * length);
                    }
                }
                list[current] = host;
                current++;
            }
        } while (!feof(file));
        fclose(file);
        if (length)
        {
            list = (struct hostInfo*) realloc(list, sizeof (struct hostInfo) *(current + 1));
        } else
        {
            list = (struct hostInfo*) malloc(sizeof (struct hostInfo));
        }
        list[current].mtype = 0;
        return list;
    }
private:
    std::string filename;
};

int main()
{
    Config cfg("network.cfg");
    unsigned short port = (unsigned short) cfg.getInt("id manager port", 6111);
    Socket * master, * connection;
    pid_t pid;
    struct idManagerMessage msg;
    size_t bytes, expectedBytes = sizeof (msg);
    char * address;
    IdManager idm("ids.dat");

    master = new Socket("net-idManager");
    master->passive(port);
    Semaphore * mutex = new Semaphore(PATH, SEM_MUTEX_IDM, "idManager");
    mutex->get();
    struct hostInfo * list = NULL;

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
            perror("fork: net-idManager.");
        } else if (pid == 0)
        {
            do
            {
                bytes = connection->receive((char*) &msg, expectedBytes);
                if (bytes == expectedBytes)
                {
                    mutex->wait();
                    switch (msg.type)
                    {
                        case GET:
                            msg.response.more = false;
                            msg.response.info = idm.get(msg.response.info.mtype);
                            mutex->post();
                            connection->send((char*) &msg, sizeof (msg));
                            break;
                        case QUERY:
                            msg.response.more = true;
                            list = idm.query();
                            mutex->post();
                            for (unsigned i = 0; list[i].mtype != 0; i++)
                            {
                                msg.response.info = list[i];
                                if (list[i + 1].mtype == 0)
                                {
                                    msg.response.more = false;
                                }
                                connection->send((char*) &msg, sizeof (msg));
                            }
                            break;
                        case REGISTER:
                            address = inet_ntoa(connection->getRemoteAddress());
                            msg.response.info.mtype = idm.doRegister(address);
                            mutex->post();
                            connection->send((char*) &msg, sizeof (msg));
                            connection->doClose();
                            exit(EXIT_SUCCESS);
                            break;
                        default:
                            mutex->post();
                            Helper::output(stderr, "ned-idManager: Wrong message received\n.", RED);
                            exit(EXIT_FAILURE);
                    }
                }
            } while (bytes == expectedBytes);
            Helper::output(stdout, "net-idManager: connection ended");
            exit(EXIT_SUCCESS);
        }
    }
    return 0;
}
