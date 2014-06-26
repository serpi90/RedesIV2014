#ifndef INCLUDES_H
#define	INCLUDES_H

#include <stddef.h>

#define ROBOT_AMOUNT 4
#define PLATFORM_CAPACITY 10
#define DISPOSITIVE_TYPES 5
#define NET_MESSAGE_SIZE 4096

#include <unistd.h>

namespace IPC {

	static const char * path = "/home/knoppix/Maestri/Ejercicio2/E2V1";

	enum class QueueIdentifier
		: int {
			INVALID_IDENTIFIER = 0,
		ARMADO_FROM_DISP_TO_CTL = 1,
		ARMADO_FROM_CTL_TO_INTERFACE = 2,
		ARMADO_BROKER = 3,
		ACTIVADO_FROM_DISP_TO_CTL = 4,
		ACTIVADO_FROM_CTL_TO_INTERFACE = 5,
		ACTIVADO_BROKER = 6,
		SALIDA_FROM_INTERFACE_TO_CTL = 7,
		SALIDA_FROM_CTL_TO_INTERFACE = 8,
		SALIDA_BROKER = 9,
		DISPOSITIVOS_FROM_PLATAFORMA_TO_CTL = 10,
		DISPOSITIVOS_FROM_CTL_TO_DISP = 11,
		DISPOSITIVOS_BROKER = 12,
		FROM_INTERFACE_TO_PLATAFORMA = 13,
		FROM_PLATAFORMA_TO_INTERFACE = 14,
		FROM_INTERFACE_TO_EXCLUSION = 15,
		FROM_EXCLUSION_TO_INTERFACE = 16,
		FROM_CTL_TO_NET = 17,
		FROM_NET_TO_CTL = 18,
		TO_ID_MANAGER = 19,
		FROM_ID_MANAGER = 20,
		TO_BROKER = 21,
		TO_BROKER_FROM_RECEIVER = 22,
		TO_SENDER_FROM_BROKER = 23
	};

	enum class SemaphoreIdentifier
		: int {
			INVALID_IDENTIFIER = 0,
		MUTEX_EXCLUSION,
		MUTEX_PLATAFORMA,
		SEM_EXCLUSION,
		SEM_ESPERA,
		MUTEX_ID_MANAGER
	};

	enum class SharedMemoryIdentifier
		: int {
			INVALID_IDENTIFIER = 0, EXCLUSION, PLATAFORMA
	};

	enum class MessageTypes
		: long {
			ANY = 0,
		ROBOT_1_ARMADO = 1,
		ROBOT_1_SACADO = 2,
		ROBOT_2 = 3,
		DISPOSITIVO = 4,
		PLATAFORMA = 5,
		BROKER = 6,
		UNWRAPPER = 7,
		FIRST_AVAILABLE_MTYPE = 8
	};
}

struct dispositivo {
		long id;
		long tipo;
};

namespace ColaExclusion {
	enum operation {
		ESPERAR_SI_SACANDO,
		AVISAR_SI_ESPERANDO_PARA_SACAR,
		ESPERAR_SI_ARMANDO,
		AVISAR_SI_ESPERANDO_PARA_ARMAR
	};

	typedef struct {
			long mtype;
			unsigned number;
			enum operation operation;
	} message;

	enum RobotStatus {
		BUSY, IDLE, WAITING
	};

	struct shared {
			enum RobotStatus armando[ROBOT_AMOUNT];
			enum RobotStatus sacando[ROBOT_AMOUNT];
	};
}

namespace ColaPlataforma {
	enum operation {
		RESERVAR, COLOCAR_DISPOSITIVO, TOMAR_DISPOSITIVO
	};

	typedef struct {
			long mtype;
			enum operation operation;
			struct dispositivo dispositivo;
			union {
					unsigned lugar;
					unsigned numero;
			};
	} message;

	enum class SlotStatus {
		RESERVED, OCCUPIED, FREE
	};

	enum class RobotStatus {
		WAITING, NOT_WAITING
	};

	struct shared {

			struct {
					struct dispositivo dispositivo;
					SlotStatus status;
			} slot[PLATFORM_CAPACITY];
			RobotStatus robotStatus[ROBOT_AMOUNT];
			unsigned amount;
	};
}

namespace ColaArmado {

	typedef struct {
			long mtype;
			struct dispositivo dispositivo;
	} message;
}

namespace ColaActivado {

	typedef struct {
			long mtype;
			struct dispositivo dispositivo;
	} message;
}

namespace ColaSalida {

	typedef struct {
			long mtype;
			struct dispositivo dispositivo;
	} message;
}

// TODO: actualizar diagrama renombrar a cola De Dispositivo
namespace ColaDispositivo {
	typedef struct {
			long mtype;
	} message;
}

namespace Broker {
	enum class Request {
		DAME_DISPOSITIVO_PARA_ARMAR,
		AVISAME_SI_ESTOY_ARMADO,
		DAME_DISPOSITIVO_PARA_SACAR_DE_PLATAFORMA,
		DAME_DISPOSITIVO_PARA_SACAR_DE_CINTA_SALIDA,
		NEW_ID
	};

	struct message {
			long mtype;
			union {
					long type;
					long connNumber;
			};
			Request request;
	};
}

namespace Net {
	enum class interfaceMessageType {
		ACTIVADO, ARMADO, DISPOSITIVO, PLATAFORMA, SALIDA, BROKER_REQUEST
	};
	struct interfaceMessage {
			long destination;
			interfaceMessageType type;
			union {

					ColaActivado::message activado;
					ColaArmado::message armado;
					ColaDispositivo::message dispositivo;
					ColaPlataforma::message plataforma;
					ColaSalida::message salida;
					Broker::message broker_request;
			};
	};

	struct message {
			size_t size;
			char message[NET_MESSAGE_SIZE];
	};
}

namespace Broker {
	struct outgoingMessage {
			long mtype;
			Net::interfaceMessage interfaceMessage;
	};
}

#endif	/* INCLUDES_H */
