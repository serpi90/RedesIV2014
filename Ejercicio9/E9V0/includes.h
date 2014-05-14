#ifndef INCLUDES_H
#define	INCLUDES_H

// Definitions

#define CANTIDAD_HIDROGENO 2

// IPC Definitions

#define PATH "/home/knoppix/Maestri/Ejercicio9/E9V0"
#define Q_FROM_INTERFACE 1
#define Q_TO_INTERFACE 2

#define SEM_MUTEX 1
#define SEM_H 2
#define SEM_H1 3
#define SEM_O 5

#define SHM_REGISTRO 1

// Message Types

#define M_HIDROGENO 1
#define M_OXIGENO 2

// Messages

enum iMessageType {
    ACTIVAR, ACTIVADO,
    TERMINAR, TERMINADO
};

enum kind {
    HIDROGENO, OXIGENO
};

struct iMessage {
    long type;
    enum kind kind;
    enum iMessageType message;
};

// Shared Memory structures

struct registro {
    short activos;
    short hidrogenos;
};

#endif	/* INCLUDES_H */
