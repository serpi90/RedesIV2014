#ifndef INCLUDES_H
#define	INCLUDES_H

// Definitions

#define CANTIDAD_EMBOTELLADORAS 3

// IPC Definitions

#define PATH "/home/knoppix/Maestri/Ejercicio8/E8V0"
#define Q_FROM_INTERFACE 1
#define Q_TO_INTERFACE 2

#define SEM_MUTEX 1
#define SEM_EMBOTELLADORAS 2

#define SHM_REGISTRO 1

// Message Types

#define M_EMBOTELLADORA 1

// Messages

enum iMessageType {
    TERMINE, CONTINUAR
};

struct iMessage {
    long type;
    enum iMessageType message;
};

// Shared Memory structures

struct registro {
    short terminaron;
};

#endif	/* INCLUDES_H */
