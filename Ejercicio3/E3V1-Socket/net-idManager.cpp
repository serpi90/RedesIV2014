#include <cstring>
#include "Socket.h"
#include "Semaphore.h"
#include "net-idManagerProtocol.h"
#include "Config.h"
#include "Helper.h"
#include "includes.h"
#include <sstream>

struct hostInfo
{
    char address[MAX_ADDRESS_LENGTH];
    long mtype;
};

class IdManager
{
public:

    IdManager(std::string filename)
    {
        this->filename = filename;

    }

    char* get(long mtype)
    {
        static struct hostInfo host;
        FILE * file = fopen(filename.c_str(), "rb");
        host.mtype = 0;
        if (file)
        {
            do
            {
                fread(&host, sizeof (host), 1, file);
            } while (!feof(file) && host.mtype != mtype);
            if (feof(file))
            {
                host.mtype = 0;
            }
            fclose(file);
        }
        return (char*) host.address;
    }

    long doRegister(char * address)
    {
        long mtype = 1;
        struct hostInfo host;
        FILE * file;
        file = fopen(filename.c_str(), "rb");
        if (file)
        {
            do
            {
                fread(&host, sizeof (host), 1, file);
            } while (!feof(file) && host.mtype == mtype++);
            fclose(file);
        }
        file = fopen(filename.c_str(), "ab");
        strncpy(host.address, address, MAX_ADDRESS_LENGTH);
        host.mtype = mtype;
        fwrite(&host, sizeof (host), 1, file);
        fclose(file);
        return mtype;

    }

    long * query()
    {
        struct hostInfo host;
        long* list;
        unsigned length = 0, current = 0;
        ;
        FILE * file = fopen(filename.c_str(), "rb");
        if (file)
        {
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
                            list = (long*) malloc(sizeof (long));
                        } else
                        {
                            length *= 2;
                            list = (long*) realloc(list, sizeof (long) * length);
                        }
                    }
                    list[current] = host.mtype;
                    current++;
                }
            } while (!feof(file));
            fclose(file);
        }
        if (length)
        {
            list = (long*) realloc(list, sizeof (long) *(current + 1));
        } else
        {
            list = (long*) malloc(sizeof (long));
        }
        list[current] = 0;
        return list;
    }
private:
    std::string filename;
};

int main()
{
    Config cfg("network.conf");
    unsigned short port = (unsigned short) cfg.getInt("id manager port", 6111);
    Socket * master, * connection;
    pid_t pid;
    struct idManagerMessage msg;
    size_t bytes, expectedBytes = sizeof (msg);
    char * address;
    long * list = NULL;
    long mtype;
    std::stringstream ss;
    IdManager * idm;

    // Prevent zombie processes.
    struct sigaction sigchld_action;
    sigchld_action.sa_handler = SIG_DFL;
    sigchld_action.sa_flags = SA_NOCLDWAIT;
    sigaction(SIGCHLD, &sigchld_action, NULL);

    idm = new IdManager("ids.dat");

    master = new Socket("net-idManager");
    if (master->passive(port) == -1)
    {
        exit(EXIT_FAILURE);
    };
    Semaphore * mutex = new Semaphore(PATH, SEM_MUTEX_IDM, "idManager");
    mutex->get();

    while (true)
    {
        connection = master->doAccept();
        if (connection == NULL)
        {
            Helper::output(stderr, "Error en accept.\n");
            exit(EXIT_FAILURE);
        }
        pid = fork();
        if (pid < 0)
        {
            perror("fork: net-idManag0er.");
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
                            msg.mtype.more = false;
                            mtype = msg.mtype.mtype;
                            memcpy(msg.address, idm->get(mtype), MAX_ADDRESS_LENGTH);
                            mutex->post();
                            ss.str("");
                            ss << "idManager: GET " << mtype << ": " << msg.address << std::endl;
                            Helper::output(stdout, ss, GREEN);
                            connection->send((char*) &msg, sizeof (msg));
                            break;
                        case QUERY:
                            msg.mtype.more = true;
                            list = idm->query();
                            mutex->post();
                            ss.str("");
                            ss << "idManager: QUERY " << std::endl;
                            Helper::output(stdout, ss, BLUE);
                            for (unsigned i = 0; msg.mtype.more; i++)
                            {
                                msg.mtype.mtype = list[i];
                                if (list[i + 1] == 0)
                                {
                                    msg.mtype.more = false;
                                }
                                connection->send((char*) &msg, sizeof (msg));
                            }
                            break;
                        case REGISTER:
                            address = inet_ntoa(connection->getRemoteAddress());
                            msg.mtype.mtype = idm->doRegister(address);
                            mutex->post();
                            ss.str("");
                            ss << "idManager: REGISTER " << address << ": " << msg.mtype.mtype << std::endl;
                            Helper::output(stdout, ss, PURPLE);
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
        } else
        {
            connection->doClose();
        }
    }
    return 0;
}
