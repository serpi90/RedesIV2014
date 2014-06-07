const MAX_ADDRESS_LENGTH = 16;
const MAX_CONSUMMERS = 3;

enum consummerType{
    DISKS=0,
    MOTHERBOARDS=1,
    PROCCESSORS=2
};

enum errorCodes {
	FILE_ACCESS_FAILURE=0,
	INVIALID_ID=1,
        UNKNOWN_CONSUMMER_TYPE=2
};

union registerResult switch(int cod_ret) {
	case  0: long mtype;
	case -1: enum errorCodes error;
	default: void;
};

union queryResult switch(int cod_ret) {
	case 0: long mtype[MAX_CONSUMMERS]; 
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
		registerResult REGISTER_CONSUMMER(enum consummerType) = 1;
		queryResult QUERY_CONSUMMERS(void) = 2;
		getResult GET(long) = 3;
                registerResult REGISTER_PRODUCER(void) = 4;
	} = 1;
} = 0x20000099;

