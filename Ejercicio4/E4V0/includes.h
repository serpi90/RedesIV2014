#ifndef INCLUDES_H
#define	INCLUDES_H

// Definitions

#define CC_SIZE 50
#define ROOM_SIZE 25

// IPC Definitions

#define PATH "/home/knoppix/Maestri/Ejercicio4/V0"
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
    WAITING, NOT_WAITING, WORKING
};

enum location {
    DOWN, UP
};

struct cableCarril {
    enum state estadoArriba;
    enum state estadoAbajo;
    enum state estadoCablecarril;
    enum location ubicacionCablecarril;
    long personasArriba[ROOM_SIZE];
    unsigned int cantArriba;
    unsigned int pReadArriba;
    unsigned int pWriteArriba;
    long personasAbajo[ROOM_SIZE];
    unsigned int cantAbajo;
    unsigned int pReadAbajo;
    unsigned int pWriteAbajo;
};

// Messages

enum iMessageType {
    ESPERAR_PERSONA_ABAJO, ESPERAR_PERSONA_ABAJO_OK,
    ESPERAR_PERSONA_ARRIBA, ESPERAR_PERSONA_ARRIBA_OK,
    ESTOY_ABAJO, ESTOY_ABAJO_OK,
    ESTOY_ARRIBA, ESTOY_ARRIBA_OK,
    CARGAR_PERSONAS_ABAJO, CARGAR_PERSONAS_ABAJO_OK,
    CARGAR_PERSONAS_ARRIBA, CARGAR_PERSONAS_ARRIBA_OK
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
#endif	/* INCLUDES_H */
