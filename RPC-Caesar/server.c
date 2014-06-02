#include "caesar.h"

char encode_c(char c, short n) {
    char lower = c | 32;
    if (lower >= 'a' && lower <= 'z') {
        if (lower + n > 'z') {
            c = c - ('z' - 'a' + 1);
        } else if (lower + n < 'a') {
            c = c + ('z' - 'a' + 1);
        }
        c = c + n;
    }
    return c;
}

caesar_output * encode_1_svc(caesar_input * in, struct svc_req * svc) {
    static caesar_output out;
    int i;
    out.ret_code = 0;
    for (i = 0; i < MAX_TEXT_LENGTH && in->text[i] != '\0'; i++) {
        out.caesar_output_u.text[i] = encode_c(in->text[i], in->shift);
    }
    if (i < MAX_TEXT_LENGTH) {
        out.caesar_output_u.text[i] = '\0';
    }
    return &out;
}

caesar_output * decode_1_svc(caesar_input * in, struct svc_req *svc) {
    static caesar_output out;
    int i;
    out.ret_code = 0;
    for (i = 0; i < MAX_TEXT_LENGTH && in->text[i] != '\0'; i++) {
        out.caesar_output_u.text[i] = encode_c(in->text[i], -in->shift);
    }
    if (i < MAX_TEXT_LENGTH) {
        out.caesar_output_u.text[i] = '\0';
    }
    return &out;
}
