#ifndef IDMANAGERPROTOCOL_H
#define	IDMANAGERPROTOCOL_H

#define MAX_ADDRESS_LENGTH 256

struct hostInfo {
    char address[MAX_ADDRESS_LENGTH];
    long mtype;
};

enum idManagerMessageType {
    REGISTER, QUERY, GET
};

struct idManagerMessage {

    union {
        enum idManagerMessageType type;

        struct {
            struct hostInfo info;
            bool more;
        } response;
    };
};

#endif	/* IDMANAGERPROTOCOL_H */
