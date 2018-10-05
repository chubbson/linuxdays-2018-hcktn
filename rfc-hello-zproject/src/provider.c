/*  =========================================================================
    provider - Provider peer

    Copyright (c) the Authors
    =========================================================================
*/

/*
@header
    provider - Provider peer
@discuss
@end
*/

#include "rfc_hello_classes.h"

int main (int argc, char **argv)
{
    if (argc < 2) {
        printf("Missing endpoint argument\n");
        exit(0);
    }
    zsock_t *rep = zsock_new_rep (argv[1]);

    hello_t *msg = NULL;
    while (!zsys_interrupted) {
        int rv = hello_recv (msg, rep);
        assert(rv == 0);

        int id = hello_id(msg);
        if (id == 3) {
            hello_set_id(msg, HELLO_OK);
            hello_set_reason(msg, "");
        } else {
            hello_set_id(msg, HELLO_ERROR);
            hello_set_reason(msg, "WRONG ENCODING");
        }
        hello_send(msg, rep);
    }
    zsock_destroy (&rep);
    hello_destroy(&msg);
    return 0;
}
