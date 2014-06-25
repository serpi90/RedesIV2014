#ifndef IDMANAGERPROTOCOL_H
#define	IDMANAGERPROTOCOL_H

#define MAX_ADDRESS_LENGTH 256

namespace IdManager {

	enum class MessageType {
		REGISTER_HOST, GET_BROKER
	};

	enum class HostKind {
		DISPOSITIVE, ROBOT1PA, ROBOT1PB, ROBOT2P, BROKER
	};

	struct register_host {
			long mtype;
			HostKind kind;
	};

	struct get_broker {
			char address[MAX_ADDRESS_LENGTH];
			long broker;
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
