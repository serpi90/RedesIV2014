const MAX_ADDRESS_LENGTH = 16;

enum errorCodes {
	FILE_ACCESS_FAILURE=0,
	INVIALID_ID=1
};

union registerResult switch(int cod_ret) {
	case  0: long mtype;
	case -1: enum errorCodes error;
	default: void;
};

union queryResult switch(int cod_ret) {
	case 0: long mtype<>; 
	case -1: enum errorCodes error;
	default: void;
};

union getResult switch(int cod_ret) {
	case 0: char address[MAX_ADDRESS_LENGTH];
	case -1: enum errorCodes error;
	default: void;
};

program IDMANAGER {
	version FIRST {
		registerResult REGISTER(void) = 1;
		queryResult QUERY(void) = 2;
		getResult GET(long) = 3;
	} = 1;
} = 0x20000099;

