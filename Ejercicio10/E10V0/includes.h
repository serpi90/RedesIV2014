#ifndef INCLUDES_H
#define	INCLUDES_H

// Definitions

#define UMBRAL 5

// IPC Definitions

#define PATH "/home/knoppix/Maestri/Ejercicio10/E10V0"
#define Q_FROM_INTERFACE 1
#define Q_TO_INTERFACE 2

#define SEM_MUTEX 1
#define SEM_LECTORES 2
#define SEM_ESCRITORES 3
#define SEM_COTIZ 4

#define SHM_REGISTRO 1

// Message Types

#define M_LECTOR 1
#define M_ESCRITOR 2

// Messages

enum iMessageType {
    QUIERO_LEER, LEE,
    QUIERO_ESCRIBIR, ESCRIBI
};

struct iMessage {
    long type;
    enum iMessageType message;
    double cotizacion;
};

// Shared Memory structures

struct registro {
    short lectores;
    short lectoresLeyendo;
    short lectoresEsperando;
    short escritorEscribiendo;
    short escritoresEsperando;
    double cotizacion;
};

#endif	/* INCLUDES_H */
