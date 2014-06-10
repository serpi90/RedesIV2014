#ifndef IDMANAGERPROTOCOL_H
#define	IDMANAGERPROTOCOL_H

#define MAX_ADDRESS_LENGTH 256

enum idManagerMessageType {
    REGISTER_CONSUMMER, REGISTER_PRODUCER, GET_CONSUMMERS, GET_HOST
};

struct get_consummers {
    long disks, processors, motherboards;
};

struct get_host {
    char address[MAX_ADDRESS_LENGTH];
};

struct register_host {
    long mtype;
};

struct idManagerMessage {
    enum idManagerMessageType type;

    union {
        struct get_consummers get_consummers;

        struct get_host get_host;

        struct register_host register_host;
    };
};

#endif	/* IDMANAGERPROTOCOL_H */
