#ifndef INCLUDES_H
#define	INCLUDES_H

#define CANT_PRODUCTORES 5
#define CANT_CONSUMIDORES 3

// IPC Definitions
#define PATH "/home/knoppix/Maestri/Ejercicio3/E3V0"

#define Q_ALMACEN 1

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
#endif	/* INCLUDES_H */
