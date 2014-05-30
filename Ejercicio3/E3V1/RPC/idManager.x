const MAX_NAME_SIZE = 128;

struct information{
    char nombre[MAX_NAME_SIZE];
};

struct request{
    long id;
    char nombre[MAX_NAME_SIZE];
};

const MAX_MESSAGE_SIZE = 512;

union infoResult
      switch (int cod_ret) {
	     case 0: struct information data;
	     case -1: char message[MAX_MESSAGE_SIZE];
             default: void;
             };

union idResult
      switch (int cod_ret) {
             case 0: long id;
	     case -1: char message[MAX_MESSAGE_SIZE];
             default: void;
             };

program IDMANAGER {
	version FIRST {
                idResult REGISTER(struct information) = 1;
                idResult GET_REGISTERED(void) = 2;
	} = 1;
} = 0x20000099;

