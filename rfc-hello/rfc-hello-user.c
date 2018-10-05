#include <czmq.h>

int
main (int argc, char **argv) {

    assert (argc >= 3);
    printf ("Endpoint: '%s'\n", argv [1]);
    printf ("Message: '%s'\n", argv [2]);

    zsock_t *socket = zsock_new_req ((const char *) argv [1]);
    assert (socket);

    int rv = zstr_send (socket, argv [2]);
    assert (rv == 0);
    printf ("Sent '%s' string to PROVIDER.\n", argv [2]);

    zmsg_t *message = zmsg_recv (socket);
    assert (message);

    printf ("Recevied: ");
    char *string = zmsg_popstr (message);
    while (string) {
        printf (" %s", string);
        string = zmsg_popstr (message);
    }
    printf ("\n");

    zsock_destroy (&socket);

    return EXIT_SUCCESS;
}
