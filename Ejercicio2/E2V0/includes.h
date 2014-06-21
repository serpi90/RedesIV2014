#ifndef INCLUDES_H
#define	INCLUDES_H

#define ROBOT_AMOUNT 4
#define PLATFORM_CAPACITY 20

namespace IPC {

	static const char * path = "/home/knoppix/Maestri/Ejercicio2/E2V0";

	enum class QueueIdentifier
		: int {
			INVALID_IDENTIFIER = 0, ARMADO, ACTIVADO, SALIDA, DISPOSITIVO, PLATAFORMA_FROM_INTERFACE, PLATAFORMA_TO_INTERFACE, EXCLUSION_FROM_INTERFACE, EXCLUSION_TO_INTERFACE
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

namespace exclusion {

	enum operation {
		ESPERAR_SI_SACANDO, AVISAR_SI_ESPERANDO_PARA_SACAR, ESPERAR_SI_ARMANDO, AVISAR_SI_ESPERANDO_PARA_ARMAR
	};

	typedef struct {
			long mtype;
			unsigned number;
			enum operation operation;
	} message;
}

namespace plataforma {

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
}

namespace colaArmado {

	typedef struct {
			long mtype;
			struct dispositivo dispositivo;
	} message;
}

namespace colaActivado {

	typedef struct {
			long mtype;
			struct dispositivo dispositivo;
	} message;
}

namespace colaSalida {

	typedef struct {
			long mtype;
			struct dispositivo dispositivo;
	} message;
}

// TODO: actualizar diagrama renombrar a cola De Dispositivo
namespace colaDispositivo {

	typedef struct {
			long mtype;
			struct dispositivo dispositivo;
	} message;
}

#endif	/* INCLUDES_H */
