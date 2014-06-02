#include "caesar.h"
#include <cstring>
#include <iostream>
#include <cstdlib>

using namespace std;

int main(int argc, char** argv) {
    CLIENT * clnt;
    caesar_input input;
    caesar_output * output;
    string host;
    short shift;
    if (argc < 4) {
        cerr << "Usage: " << argv[0] << " <host> <shift> <text>" << endl;
        exit(EXIT_FAILURE);
    }
    host = argv[1];
    shift = (short) atoi(argv[2]);

    clnt = clnt_create(host.c_str(), CAESAR, VERSION, "tcp");
    if (clnt == NULL) {
        clnt_pcreateerror(host.c_str());
        exit(EXIT_FAILURE);
    }
    input.shift = shift;
    strncpy(input.text, argv[3], MAX_TEXT_LENGTH);
    output = encode_1(&input, clnt);
    if (output == (caesar_output*) NULL) {
        clnt_perror(clnt, "call failed");
    }
    if (output->ret_code != 0) {
        fprintf(stderr, "Encode failed %d\n", output->ret_code);
        exit(EXIT_FAILURE);
    }
    cout << "Encoded: " << output->caesar_output_u.text << endl;

    input.shift = shift;
    strncpy(input.text, output->caesar_output_u.text, MAX_TEXT_LENGTH);
    output = decode_1(&input, clnt);
    if (output == (caesar_output*) NULL) {
        clnt_perror(clnt, "call failed");
    }
    if (output->ret_code != 0) {
        fprintf(stderr, "Encode failed %d\n", output->ret_code);
        exit(EXIT_FAILURE);
    }
    cout << "Decoded: " << output->caesar_output_u.text << endl;
    clnt_destroy(clnt);
    return (EXIT_SUCCESS);
}

