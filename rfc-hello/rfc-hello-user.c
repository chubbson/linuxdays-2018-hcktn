#include <czmq.h>

int
main (int argc, char **argv) {

    assert (argc >= 2);
    printf ("Endpoint: '%s'\n", argv [1]);

    zsock_t *socket = zsock_new_req ((const char *) argv [1]);
    assert (socket);

    int rv = zstr_send (socket, "HELLO");
    assert (rv == 0);
    printf ("Sent 'HELLO' string to PROVIDER.\n");

    zmsg_t *message = zmsg_recv (socket);
    assert (message);

    printf ("Recevied: ");
    char *string = zmsg_popstr (message);
    while (string) {
        printf (" %s", string);
        zstr_free (&string);
    }
    printf ("\n");

    zsock_destroy (&socket);

    return EXIT_SUCCESS;
}
