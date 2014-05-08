#ifndef INCLUDES_H
#define	INCLUDES_H

// Definitions

#define CC_SIZE 50
#define ROOM_SIZE 25

// IPC Definitions

#define PATH "/home/knoppix/Maestri/Ejercicio4/E4V0"
#define Q_SALA_TO_INTERFACE 1
#define Q_SALA_FROM_INTERFACE Q_SALA_TO_INTERFACE + 1
#define Q_CC_TO_INTERFACE Q_SALA_FROM_INTERFACE + 1
#define Q_CC_FROM_INTERFACE Q_CC_TO_INTERFACE + 1
#define Q_PERSONA Q_CC_FROM_INTERFACE + 1
#define Q_SALA_ABAJO Q_PERSONA + 1
#define Q_SALA_ARRIBA Q_SALA_ABAJO + 1
#define Q_CC Q_SALA_ARRIBA + 1
#define SEM_MUTEX Q_CC + 1
#define SEM_FULL SEM_MUTEX + 1
#define SEM_EMPTY SEM_FULL + 1
#define SHM_CC SEM_EMPTY + 1
// Message Types

#define M_SALA_ABAJO 1
#define M_SALA_ARRIBA M_SALA_ABAJO + 1
#define M_CABLE_CARRIL M_SALA_ARRIBA + 1
#define M_PERS M_CABLE_CARRIL + 1

// Shared memory structures

enum state {
    WAITING, WORKING, TRAVELLING
};

enum location {
    BOTTOM, TOP
};

struct sala {
    enum state estado;
    long personas[ROOM_SIZE];
    unsigned int pRead;
    unsigned int pWrite;
    unsigned int cantidad;
};

struct cablecarril {
    enum state estado;
    enum location ubicacion;
};

struct registro {
    struct cablecarril cc;
    struct sala arriba;
    struct sala abajo;
};

// Messages

enum iMessageType {
    ESPERAR_PERSONA_ABAJO, ENTRO_PERSONA_ABAJO,
    ESPERAR_PERSONA_ARRIBA, ENTRO_PERSONA_ARRIBA,
    ESTOY_ABAJO, BAJARON_PASAJEROS_ABAJO,
    ESTOY_ARRIBA, BAJARON_PASAJEROS_ARRIBA,
    CARGAR_PERSONAS_ABAJO, TERMINE_DE_CARGAR_PERSONAS_ABAJO,
    CARGAR_PERSONAS_ARRIBA, TERMINE_DE_CARGAR_PERSONAS_ARRIBA
};

union iMessageContent {
    long people[CC_SIZE];
};

struct iMessage {
    long type;
    long sender;
    enum iMessageType message;
    union iMessageContent data;
};

enum personMessageType {
    QUIERO_ENTRAR, ENTRA,
    SUBIR, SUBI,
    BAJAR, BAJE
};

struct personMessage {
    long type;
    long sender;
    enum personMessageType message;
};

#include <cstdio>
#include <sstream>
#include <string>
#include <unistd.h>

#endif	/* INCLUDES_H */
