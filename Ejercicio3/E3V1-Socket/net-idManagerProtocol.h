#ifndef IDMANAGERPROTOCOL_H
#define	IDMANAGERPROTOCOL_H

#define MAX_ADDRESS_LENGTH 256

enum idManagerMessageType {
    REGISTER, QUERY, GET
};

struct idManagerMessage {
    enum idManagerMessageType type;

    union {
        char address[MAX_ADDRESS_LENGTH];

        struct {
            long mtype;
            bool more;
        } mtype;
    };
};

#endif	/* IDMANAGERPROTOCOL_H */
