#include <malamute.h>


//  actor command ENDPOINT / "<endpoint>"
//  actor command SEND / "<stream name>" / "<string>"

static void
publisher_actor (zsock_t *pipe, void *args)
{
    assert (pipe);

    zpoller_t *poller = zpoller_new (pipe, NULL);
    assert (poller);
    mlm_client_t *client = mlm_client_new ();
    assert (client);

    zsock_signal (pipe, 0);

    while (!zsys_interrupted) {
        void *which = zpoller_wait (poller, 1000);


        if (which == NULL) {
            continue;
        }

        if (which == pipe) {
            zmsg_t *message = zmsg_recv (pipe);
            if (!message)
                break;

            char *command = zmsg_popstr (message);
            if (!command) {
                zmsg_destroy (&message);
            }

            if (streq (command, "ENDPOINT")) {
                int rv = mlm_client_connect ();
                assert (rv == 0);
            }
            else
            if (streq (command, "SEND")) {
            }
            else {
            }

            zstr_free (&command);
            zmsg_destroy (&message);

            
        }

    }
    zpoller_festroy (&poller);
    mlm_client_destroy (&client);
}

static const char *endpoint = "inproc://@/florian"

int
main (int argc, char **argv) {

    zactor_t *publisher = zactor_new (publisher_actor, NULL);
    assert (publisher);

    zactor_t *server = zactor_new (mlm_server, "malamute");
    assert (server);

    zstr_send (server, "VERBOSE");
    zstr_sendx (server, "BIND", endpoint, NULL);
    zstr_sendx (server, "SET", "server/timeout", "10000", NULL);    

    zclock_sleep (500);

    mlm_client_t *listener = mlm_client_new ();
    assert (client);

    mlm_client_connect ();
    mlm_client_set_consumer ();


    zstr_sendx (publisher, "ENDPOINT", endpoint, NULL);
    zslock_sleep (100);

    zstr_sendx (publisher, "SEND", "upses", "Karol", NULL);

    zmsg_t *msg = mlm_client_recv ();

    char *soemthing =  zmsg_popstr ();
    assert (





    zactor_destroy (&publisher);
    return EXIT_SUCCESS;

    //  Test 1
    {
    }
    //  Test 2
    {
    }
}
