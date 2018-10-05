#include <czmq.h>
#include <zmq.h>
#include <stdlib.h>
#include <stdio.h>

int main (int argc, char **argv)
{
    if (argc < 2) {
        printf("Missing endpoint argument\n");
        exit(0);
    }
    zsock_t *rep = zsock_new_rep (argv[1]);

    while (!zsys_interrupted) {
        char *string = zstr_recv(rep);
        if (strcmp(string,"HELLO") == 0) {
            zstr_send(rep, "OK");
        } else {
            zstr_sendx(rep, "ERROR", "WRONG ENCODING", NULL);
        }
        zstr_free (&string);
    }
    zsock_destroy (&rep);
    return 0;
}
