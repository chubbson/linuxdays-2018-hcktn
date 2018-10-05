/*  =========================================================================
    rfc_hello_user - User peer

    Copyright (c) the Authors
    =========================================================================
*/

/*
@header
    rfc_hello_user - User peer
@discuss
@end
*/

#include "rfc_hello_classes.h"

int
main (int argc, char **argv) {

    assert (argc >= 2);
    printf ("Endpoint: '%s'\n", argv [1]);

    zsock_t *socket = zsock_new_req ((const char *) argv [1]);
    assert (socket);

    hello_t *msg = hello_new();
    hello_set_id(msg, HELLO_HELLO);
    hello_set_reason(msg, "");

    int rv = hello_send (msg, socket);
    assert (rv == 0);
    printf ("Sent 'HELLO' string to PROVIDER.\n");

    rv = hello_recv (msg, socket);
    assert (rv == 0);
    assert (msg);


    hello_print (msg);
    hello_destroy (&msg);
    zsock_destroy (&socket);

    return EXIT_SUCCESS;
}
