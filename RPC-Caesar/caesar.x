const MAX_TEXT_LENGTH = 256;
union caesar_output
	switch( int ret_code ) {
		case 0: char text[MAX_TEXT_LENGTH];
		default: void;
};

struct caesar_input {
	char text[MAX_TEXT_LENGTH];
	short shift;
};

program CAESAR {
	version VERSION {
		caesar_output encode ( caesar_input ) = 1;
		caesar_output decode ( caesar_input ) = 2;
	}=1;
}=0x20000098;
