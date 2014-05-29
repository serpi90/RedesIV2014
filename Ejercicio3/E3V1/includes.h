#ifndef INCLUDES_H
#define	INCLUDES_H

// Definitions

#define CANT_PRODUCTORES 5
#define CANT_CONSUMIDORES 3

// IPC Definitions

#define PATH "/home/knoppix/Maestri/Ejercicio3/E3V1"
#define Q_FROM_INTERFACE_TO_NET 1
#define Q_FROM_NET_TO_COMPONENT 2
#define Q_FROM_COMPONENT_TO_NET 3
#define Q_FROM_NET_TO_INTERFACE 4

#define Q_ALMACEN 3

// Message Types

#define M_CONS 1
#define M_PROD (M_CONS + CANT_CONSUMIDORES + 1)

// Messages

enum encargados {
    DISCOS = 0, PROCESADORES = 1, MOTHERBOARDS = 2
};

enum iMessageType {
    PRODUCIR_ORDEN,
    CONSUMIR_ORDEN, CONSUMISTE_ORDEN
};

struct orden {
    unsigned discos;
    unsigned procesadores;
    unsigned motherboards;
};

union iMessageContent {
    struct orden orden;
    enum encargados tipoEncargado;
};

struct iMessage {
    long type;
    enum iMessageType message;
    union iMessageContent data;
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
