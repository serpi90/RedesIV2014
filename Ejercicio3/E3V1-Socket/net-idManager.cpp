#include <cstring>
#include "Socket.h"
#include "Semaphore.h"
#include "net-idManagerProtocol.h"
#include "Config.h"
#include "Helper.h"
#include "includes.h"
#include <sstream>
#include <rpc/types.h>

#define HOST_KIND_AMOUNT (CONSUMMER_KIND_AMOUNT + 1)

struct host_info {
    char address[MAX_ADDRESS_LENGTH];
    long mtype;
};

class IdManager {
public:

    IdManager(std::string filename) {
        this->filename = filename;
        this->data = readFile(filename);
    }

    ~IdManager() {
        clearData(&data);
    }

    struct get_host getHost(long mtype) {
        struct get_host result;
        static struct hostInfo host;
        unsigned i, j;
        struct hostArray temp[HOST_KIND_AMOUNT];
        data2array(&data, temp);
        host.mtype = 0;
        for (i = 0; i < HOST_KIND_AMOUNT && host.mtype == 0; i++) {
            for (j = 0; j < temp[i].amount && host.mtype == 0; j++) {
                if (temp[i].hosts[j].mtype == *mtype) {
                    host = temp[i].hosts[j];
                }
            }
        }
        strncpy(result.address, host.address, MAX_ADDRESS_LENGTH);
        return result;
    }

    long registerConsummer(char * address, enum consummerType type) {
        char * ctype;
        struct hostInfo host;
        register_host result;
        std::stringstream ss;

        host.mtype = data.nextMtype;
        strncpy(host.address, address, MAX_ADDRESS_LENGTH);
        switch (type) {
            case DISKS:
                addHost(&(data.disks), host);
                ctype = "DISKS";
                break;
            case PROCCESSORS:
                addHost(&(data.processors), host);
                ctype = "PROCESSORS";
                break;
            case MOTHERBOARDS:
                addHost(&(data.motherboards), host);
                ctype = "MOTHERBOARDS";
                break;
            default:
                result.mtype = 0;
                ss.str("");
                ss << "Consummer Type Desconocido: " << type << std::endl;
                Helper::output(stderr, ss, RED);
                return result;
        }
        result.mtype = host.mtype;
        data.nextMtype++;
        if (!writeFile(filename, data)) {
            perror("writeFile");
            Helper::output(stderr, "Error al escribir en archivo\n", RED);
            return &result;
        }
        printf("REGISTER CONSUMMER %s as id %3ld from %s\n", ctype, host.mtype, host.address);
        return result;
    }

    struct register_host registerProducer(char * address) {
        struct register_host result;
        struct hostInfo host;
        host.mtype = data.nextMtype;
        strncpy(host.address, address, MAX_ADDRESS_LENGTH);
        addHost(&(data.producers), host);
        result.mtype = host.mtype;
        data.nextMtype++;
        if (!writeFile(filename, data)) {
            perror("writeFile");
            Helper::output(stderr, "Error al escribir en archivo\n", RED);
        }
        printf("REGISTER PRODUCER as id %3ld from %s\n", host.mtype, host.address);
        return result;
    }

    struct get_consummers query() {
        struct get_consummers result;
        struct hostArray temp[HOST_KIND_AMOUNT];
        unsigned idx[CONSUMMER_KIND_AMOUNT], i;

        if (data.disks.amount) {
            result.disks = data.disks.hosts[data.disks.lastUsed].mtype;
            data.disks.lastUsed = (data.disks.lastUsed + 1) % data.disks.amount;
        }
        if (data.processors.amount) {
            result.processors = data.processors.hosts[data.processors.lastUsed].mtype;
            data.processors.lastUsed = (data.processors.lastUsed + 1) % data.processors.amount;
        }
        if (data.motherboards.amount) {
            result.motherboards = data.motherboards.hosts[data.motherboards.lastUsed].mtype;
            data.motherboards.lastUsed = (data.motherboards.lastUsed + 1) % data.motherboards.amount;
        }
        if (!writeFile(filename, data)) {
            perror("writeFile");
            Helper::output(stderr, "Error al escribir en archivo\n", RED);
        }

        printf("QUERY: %ld %ld %ld\n", result.disks, result.processors, result.motherboards);

        return &result;
    }
private:
    std::string filename;
    struct fileData data;

    struct hostInfo {
        char address[MAX_ADDRESS_LENGTH];
        long mtype;
    };

    struct hostArray {
        unsigned amount;
        unsigned lastUsed;
        struct hostInfo * hosts;
    };

    struct fileData {
        long nextMtype;
        struct hostArray producers, disks, processors, motherboards;
    };

    void data2array(struct fileData * data, struct hostArray arr[4]) {
        arr[0] = data->producers;
        arr[1] = data->disks;
        arr[2] = data->processors;
        arr[3] = data->motherboards;
    }

    void array2data(struct fileData * data, struct hostArray arr[4]) {
        data->producers = arr[0];
        data->disks = arr[1];
        data->processors = arr[2];
        data->motherboards = arr[3];
    }

    void addHost(struct hostArray *array, struct hostInfo host) {
        if (array->hosts) {
            array->hosts = realloc(array->hosts, (array->amount + 1) * sizeof (struct hostInfo));
        } else {
            array->hosts = malloc(sizeof (struct hostInfo));
        }
        array->hosts[array->amount++] = host;
    }

    struct fileData readFile(const char * filename) {
        unsigned i;
        struct hostArray temp[HOST_KIND_AMOUNT];
        FILE * file = fopen(filename, "rb");
        if (file) {
            if (fread(&(data.nextMtype), sizeof (data.nextMtype), 1, file)) {
                for (i = 0; i < HOST_KIND_AMOUNT; i++) {
                    fread(&(temp[i].amount), sizeof (temp[i].amount), 1, file);
                    fread(&(temp[i].lastUsed), sizeof (temp[i].lastUsed), 1, file);
                }
                for (i = 0; i < HOST_KIND_AMOUNT; i++) {
                    if (temp[i].amount) {
                        temp[i].hosts = malloc(sizeof (struct hostInfo) * temp[i].amount);
                        fread(temp[i].hosts, sizeof (struct hostInfo), temp[i].amount, file);
                    } else {
                        temp[i].hosts = NULL;
                    }
                }
                array2data(&data, temp);
            } else {
                bzero(&data, sizeof (data));
                data.nextMtype = M_PROD + 1;
            }
            fclose(file);
        } else {
            bzero(&data, sizeof (data));
            data.nextMtype = M_PROD + 1;
        }
        return data;
    }

    bool_t writeFile(const char * filename, struct fileData data) {
        FILE * file = fopen(filename, "wb");
        struct hostArray temp[HOST_KIND_AMOUNT];
        unsigned i;
        data2array(&data, temp);
        if (file) {
            fwrite(&(data.nextMtype), sizeof (data.nextMtype), 1, file);
            for (i = 0; i < HOST_KIND_AMOUNT; i++) {
                fwrite(&(temp[i].amount), sizeof (temp[i].amount), 1, file);
                fwrite(&(temp[i].lastUsed), sizeof (temp[i].lastUsed), 1, file);
            }
            for (i = 0; i < HOST_KIND_AMOUNT; i++) {
                if (temp[i].amount) {

                    fwrite(temp[i].hosts, sizeof (struct hostInfo), temp[i].amount, file);
                }
            }
            fclose(file);
        } else {
            return FALSE;
        }
        return TRUE;
    }

    void clearData(struct fileData* dataPtr) {
        struct hostArray temp[HOST_KIND_AMOUNT];
        unsigned i;
        data2array(dataPtr, temp);

        dataPtr->nextMtype = M_CONS + 1;
        for (i = 0; i < HOST_KIND_AMOUNT; i++) {
            if (temp[i].hosts) {
                free(temp[i].hosts);
            }
            temp[i].hosts = (struct hostInfo*) NULL;
            temp[i].amount = 0;
            temp[i].lastUsed = 0;
        }
        array2data(dataPtr, temp);
    }
};

int main() {
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
                        case GET_HOST:
                            msg.get_host = idm->getHost(msg.type);
                            mutex->post();
                            ss.str("");
                            ss << "idManager: GET " << msg.type << ": " << msg.get_host.address << std::endl;
                            Helper::output(stdout, ss, GREEN);
                            connection->send((char*) &msg, sizeof (msg));
                            break;
                        case GET_CONSUMMERS:
                            msg.get_consummers = idm->query();
                            mutex->post();
                            ss.str("");
                            ss << "idManager: QUERY " << std::endl;
                            Helper::output(stdout, ss, BLUE);
                            connection->send((char*) &msg, sizeof (msg));
                            break;
                        case REGISTER_CONSUMMER:
                            address = inet_ntoa(connection->getRemoteAddress());
                            msg.register_host.mtype = idm->registerConsummer(address);
                            mutex->post();
                            ss.str("");
                            ss << "idManager: REGISTER CONSUMMER" << address << ": " << msg.register_host.mtype << std::endl;
                            Helper::output(stdout, ss, PURPLE);
                            connection->send((char*) &msg, sizeof (msg));
                            connection->doClose();
                            exit(EXIT_SUCCESS);
                            break;
                        case REGISTER_PRODUCER:
                            address = inet_ntoa(connection->getRemoteAddress());
                            msg.register_host.mtype = idm->registerProducer(address);
                            mutex->post();
                            ss.str("");
                            ss << "idManager: REGISTER PRODUCER" << address << ": " << msg.register_host.mtype << std::endl;
                            Helper::output(stdout, ss, PURPLE);
                            connection->send((char*) &msg, sizeof (msg));
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
        } else {
            connection->doClose();
        }
    }
    return 0;
}
