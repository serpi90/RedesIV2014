#include <cstring>
#include "Socket.h"
#include "Semaphore.h"
#include "net-idManagerProtocol.h"
#include "Config.h"
#include "Helper.h"
#include "includes.h"
#include <sstream>

class IdManager {
public:

    IdManager(std::string filename) {
        this->filename = filename;

    }

    struct hostInfo get(long mtype) {
        struct hostInfo host;
        FILE * file = fopen(filename.c_str(), "rb");
        host.mtype = 0;
        if (file) {
            do {
                fread(&host, sizeof (host), 1, file);
            } while (!feof(file) && host.mtype != mtype);
            if (feof(file)) {
                host.mtype = 0;
            }
            fclose(file);
        }
        return host;
    }

    long doRegister(char * address) {
        long mtype = 1;
        struct hostInfo host;
        FILE * file;
        file = fopen(filename.c_str(), "rb");
        if (file) {
            do {
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

    struct hostInfo * query() {
        struct hostInfo * list = NULL, host;
        unsigned length = 0, current = 0;
        ;
        FILE * file = fopen(filename.c_str(), "rb");
        if (file) {
            do {
                fread(&host, sizeof (host), 1, file);
                if (!feof(file)) {
                    if (current >= length) {
                        if (length == 0) {
                            length = 1;
                            list = (struct hostInfo*) malloc(sizeof (struct hostInfo));
                        } else {
                            length *= 2;
                            list = (struct hostInfo*) realloc(list, sizeof (struct hostInfo) * length);
                        }
                    }
                    list[current] = host;
                    current++;
                }
            } while (!feof(file));
            fclose(file);
        }
        if (length) {
            list = (struct hostInfo*) realloc(list, sizeof (struct hostInfo) *(current + 1));
        } else {
            list = (struct hostInfo*) malloc(sizeof (struct hostInfo));
        }
        list[current].mtype = 0;
        return list;
    }
private:
    std::string filename;
};

int main() {
    Config cfg("network.conf");
    unsigned short port = (unsigned short) cfg.getInt("id manager port", 6111);
    Socket * master, * connection;
    pid_t pid;
    struct idManagerMessage msg;
    size_t bytes, expectedBytes = sizeof (msg);
    char * address;
    struct hostInfo * list = NULL;
    std::stringstream ss;
    IdManager * idm;
    idm = new IdManager("ids.dat");

    master = new Socket("net-idManager");
    if (master->passive(port) == -1) {
        exit(EXIT_FAILURE);
    };
    Semaphore * mutex = new Semaphore(PATH, SEM_MUTEX_IDM, "idManager");
    mutex->get();

    while (true) {
        connection = master->doAccept();
        if (connection == NULL) {
            Helper::output(stderr, "Error en accept.\n");
            exit(EXIT_FAILURE);
        }
        pid = fork();
        if (pid < 0) {
            perror("fork: net-idManag0er.");
        } else if (pid == 0) {
            do {
                bytes = connection->receive((char*) &msg, expectedBytes);
                if (bytes == expectedBytes) {
                    mutex->wait();
                    switch (msg.type) {
                        case GET:
                            msg.response.more = false;
                            msg.response.info = idm->get(msg.response.info.mtype);
                            mutex->post();
                            ss.str("");
                            ss << "idManager: GET " << msg.response.info.mtype << " " << msg.response.info.address << std::endl;
                            Helper::output(stdout, ss, GREEN);
                            connection->send((char*) &msg, sizeof (msg));
                            break;
                        case QUERY:
                            msg.response.more = true;
                            list = idm->query();
                            mutex->post();
                            ss << "idManager: QUERY " << std::endl;
                            for (unsigned i = 0; msg.response.more; i++) {
                                msg.response.info = list[i];
                                if (list[i + 1].mtype == 0) {
                                    msg.response.more = false;
                                }
                                connection->send((char*) &msg, sizeof (msg));
                            }
                            break;
                        case REGISTER:
                            address = inet_ntoa(connection->getRemoteAddress());
                            msg.response.info.mtype = idm->doRegister(address);
                            mutex->post();
                            ss.str("");
                            ss << "idManager: REGISTER " << address << " as " << msg.response.info.mtype << std::endl;
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
        }
    }
    return 0;
}
