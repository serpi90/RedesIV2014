#ifndef INCLUDES_H
#define	INCLUDES_H

// IPC Definitions

#define PATH "/home/knoppix/Maestri/Ejercicio7/E7V0"
#define Q_FROM_INTERFACE 11
#define Q_TO_INTERFACE 12
#define Q_FROM_IREPOSITOR 13
#define Q_TO_IREPOSITOR 14

#define SEM_PINTAR 1
#define SEM_PINTAR2 2
#define SEM_SECAR 3
#define SEM_SECAR2 4
#define SEM_MOTOR 5
#define SEM_MOTOR2 6
#define SEM_INTERIOR 7
#define SEM_INTERIOR2 8
#define SEM_EXTERIOR 9
#define SEM_EXTERIOR2 10

// Message Types

#define M_CONSTRUCCION 1
#define M_PINTADO 2
#define M_SECADO 3
#define M_COLOCADO_MOTOR 4
#define M_COLOCADO_INTERIOR 5
#define M_COLOCADO_EXTERIOR 6
#define M_REPOSITOR 7
// Messages

enum iMessageType {
    ENTREGAR_CHASIS, ENTREGAR_CHASIS_OK,
    RECIBIR_CHASIS, RECIBIR_CHASIS_OK,
    RECIBIDO_CHASIS,

    ENTREGAR_PINTADO, ENTREGAR_PINTADO_OK,
    RECIBIR_PINTADO, RECIBIR_PINTADO_OK,
    RECIBIDO_PINTADO,

    ENTREGAR_SECADO, ENTREGAR_SECADO_OK,
    RECIBIR_SECADO, RECIBIR_SECADO_OK,
    RECIBIDO_SECADO,

    ENTREGAR_MOTOR, ENTREGAR_MOTOR_OK,
    RECIBIR_MOTOR, RECIBIR_MOTOR_OK,
    RECIBIDO_MOTOR,

    ENTREGAR_INTERIORES, ENTREGAR_INTERIORES_OK,
    RECIBIR_INTERIORES, RECIBIR_INTERIORES_OK,
    RECIBIDO_INTERIORES,

    REPONER, REPONER_OK
};

enum materiales {
    MATERIALES,
    PINTURA,
    MOTOR,
    INTERIORES, RUEDA,
    PUERTA, CAPO, BAUL
};

union iMessageContent {
    enum materiales aReponer;
    unsigned cantidad;
};

struct iMessage {
    long type;
    enum iMessageType message;
    union iMessageContent data;
    
};

#endif	/* INCLUDES_H */
