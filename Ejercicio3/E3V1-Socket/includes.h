#ifndef INCLUDES_H
#define	INCLUDES_H

#define CANT_PRODUCTORES 5
#define CONSUMMER_KIND_AMOUNT 3

// IPC Definitions
#define PATH "/home/knoppix/Maestri/Ejercicio3/E3V1-Socket"
#define Q_FROM_INTERFACE_TO_CTL 1
#define Q_FROM_CTL_TO_NET 2
#define Q_FROM_NET_TO_INTERFACE 3

#define SEM_MUTEX_IDM 1

// Message Types
#define M_ANY 0
#define M_PROD 1
#define M_CONS 2

// Messages

enum consummerType {
    DISKS = 0,
    MOTHERBOARDS = 1,
    PROCCESSORS = 2
};

struct orden {
    unsigned discos;
    unsigned procesadores;
    unsigned motherboards;
};

struct mtypes {
    long disks;
    long processors;
    long motherboards;
};

struct iMessage {
    long mtype;

    union {
        struct orden orden;

        struct {
            short query;

            union {
                long id;
                struct mtypes consumidores;
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
