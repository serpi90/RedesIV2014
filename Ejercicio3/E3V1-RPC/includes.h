#ifndef INCLUDES_H
#define	INCLUDES_H

#define CANT_PRODUCTORES 5
#define CANT_CONSUMIDORES 3

// IPC Definitions
#define PATH "/home/knoppix/Maestri/Ejercicio3/E3V1-Socket"
#define Q_FROM_INTERFACE_TO_CTL 1
#define Q_FROM_CTL_TO_NET 2
#define Q_FROM_NET_TO_INTERFACE 3

#define SEM_MUTEX_IDM 1

// Message Types
#define M_CONS 1

// Messages

struct orden {
    unsigned discos;
    unsigned procesadores;
    unsigned motherboards;
};

struct msgAlmacen {
    long type;
    struct orden orden;
};

struct netMessage {
    short size;
    char message[4096];
};
#endif	/* INCLUDES_H */
