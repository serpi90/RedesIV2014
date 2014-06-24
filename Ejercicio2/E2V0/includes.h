#ifndef INCLUDES_H
#define	INCLUDES_H

#define ROBOT_AMOUNT 4
#define PLATFORM_CAPACITY 10
#define DISPOSITIVE_TYPES 5

namespace IPC {

	static const char * path = "/home/knoppix/Maestri/Ejercicio2/E2V0";

	enum class QueueIdentifier
		: int {
			INVALID_IDENTIFIER = 0, ARMADO, ACTIVADO, SALIDA, DISPOSITIVOS, PLATAFORMA_FROM_INTERFACE, PLATAFORMA_TO_INTERFACE, EXCLUSION_FROM_INTERFACE, EXCLUSION_TO_INTERFACE
	};

	enum class SemaphoreIdentifier
		: int {
			INVALID_IDENTIFIER = 0, MUTEX_EXCLUSION, MUTEX_PLATAFORMA, SEM_EXCLUSION, SEM_ESPERA
	};

	enum class SharedMemoryIdentifier
		: int {
			INVALID_IDENTIFIER = 0, EXCLUSION, PLATAFORMA
	};

	enum class MessageTypes
		: long {
			M_ANY = 0
	};
}

struct dispositivo {
		long id;
		long tipo;
};

namespace ColaExclusion {

	enum operation {
		ESPERAR_SI_SACANDO, AVISAR_SI_ESPERANDO_PARA_SACAR, ESPERAR_SI_ARMANDO, AVISAR_SI_ESPERANDO_PARA_ARMAR
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
		RESERVAR, COLOCAR_DISPOSITIVO, ESPERAR_DISPOSITIVO, TOMAR_DISPOSITIVO
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

#endif	/* INCLUDES_H */
