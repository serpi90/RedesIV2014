#ifndef IDMANAGERPROTOCOL_H
#define	IDMANAGERPROTOCOL_H

#define MAX_ADDRESS_LENGTH 256

namespace IdManager {

	enum class MessageType {
		REGISTER_HOST, GET_BROKER
	};

	enum class HostKind {
		DISPOSITIVO, ROBOT_1_ARMADO, ROBOT_1_SACADO, ROBOT_2, BROKER
	};

	struct register_host {
			long mtype;
			HostKind kind;
	};

	struct get_broker {
			char address[MAX_ADDRESS_LENGTH];
			long broker;
	};

	struct messageRequest {
			long mtype;
			HostKind kind;
	};

	struct messageReply {
			long mtype;
			long id;
	};

	struct message {
			MessageType type;

			union {
					struct get_broker get_broker;
					struct register_host register_host;
			};
	};
}
#endif	/* IDMANAGERPROTOCOL_H */
