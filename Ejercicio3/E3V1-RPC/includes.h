#ifndef INCLUDES_H
#define	INCLUDES_H

#include "RPC/idManager.h"

#define CANT_PRODUCTORES 5
#define CANT_CONSUMIDORES 3

/* IPC Definitions */
#define PATH "/home/knoppix/Maestri/Ejercicio3/E3V1-RPC"
#define Q_FROM_INTERFACE_TO_CTL 1
#define Q_FROM_CTL_TO_NET 2
#define Q_FROM_NET_TO_INTERFACE 3

/* Message Types */
#define M_ANY 0
#define M_CONS 1
#define M_PROD 2

struct orden {
    unsigned discos;
    unsigned procesadores;
    unsigned motherboards;
};

struct iMessage {
    long mtype;

    union {
        struct orden orden;

        struct {
            short query;

            union {
                long id;
                long consumidores[CANT_CONSUMIDORES];
                enum consummerType type;
            };
        } query;
    };
};

struct netMessage {
    short size;
    char message[4096];
};
#endif	/* INCLUDES_H */
