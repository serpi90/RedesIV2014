#include <stdio.h>
#include <stdlib.h>
#include "caesar.h"

int main(int argc, char** argv) {
    CLIENT * clnt;
    caesar_input rpc_input;
    caesar_output * rpc_output;
    char * host;
    short shift;
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <host> <shift> <text>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    host = argv[1];
    shift = (short) atoi(argv[2]);

    clnt = clnt_create(host, CAESAR, VERSION, "tcp");
    if (clnt == NULL) {
        clnt_pcreateerror(host);
        exit(EXIT_FAILURE);
    }

    rpc_input.shift = shift;
    strncpy(rpc_input.text, argv[3], MAX_TEXT_LENGTH);
    rpc_output = encode_1(&rpc_input, clnt);
    if (rpc_output == (caesar_output*) NULL) {
        clnt_perror(clnt, "call failed");
    }
    if (rpc_output->ret_code != 0) {
        fprintf(stderr, "Encode failed %d\n", rpc_output->ret_code);
        exit(EXIT_FAILURE);
    }
    printf("Encoded: %s\n", rpc_output->caesar_output_u.text);

    rpc_input.shift = shift;
    strncpy(rpc_input.text, rpc_output->caesar_output_u.text, MAX_TEXT_LENGTH);
    rpc_output = decode_1(&rpc_input, clnt);
    if (rpc_output == (caesar_output*) NULL) {
        clnt_perror(clnt, "call failed");
    }
    if (rpc_output->ret_code != 0) {
        fprintf(stderr, "Encode failed %d\n", rpc_output->ret_code);
        exit(EXIT_FAILURE);
    }
    printf("Decoded: %s\n", rpc_output->caesar_output_u.text);
    clnt_destroy(clnt);
    return (EXIT_SUCCESS);
}

