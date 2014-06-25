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
		ARMADO_FROM_DISP_TO_CTL,
		ARMADO_FROM_CTL_TO_INTERFACE,
		ACTIVADO_FROM_DISP_TO_CTL,
		ACTIVADO_FROM_CTL_TO_INTERFACE,
		SALIDA_FROM_INTERFACE_TO_CTL,
		SALIDA_FROM_CTL_TO_INTERFACE,
		DISPOSITIVOS_FROM_PLATAFORMA_TO_CTL,
		DISPOSITIVOS_FROM_CTL_TO_DISP,
		FROM_INTERFACE_TO_PLATAFORMA,
		FROM_PLATAFORMA_TO_INTERFACE,
		FROM_INTERFACE_TO_EXCLUSION,
		FROM_EXCLUSION_TO_INTERFACE,
		FROM_CTL_TO_NET,
		FROM_NET_TO_CTL,
		TO_ID_MANAGER,
		FROM_ID_MANAGER
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
		FIRST_AVAILABLE_MTYPE = 6
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

namespace Net {
	enum class iMessageType {
		ACTIVADO, ARMADO, DISPOSITIVO, PLATAFORMA, SALIDA
	};
	struct iMessage {
			iMessageType type;
			union {

					ColaActivado::message activado;
					ColaArmado::message armado;
					ColaDispositivo::message dispositivo;
					ColaPlataforma::message plataforma;
					ColaSalida::message salida;
			};
	};

	struct message {
			size_t size;
			char message[NET_MESSAGE_SIZE];
	};
}

#endif	/* INCLUDES_H */
