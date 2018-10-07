#include <malamute.h>

static const char *endpoint = "inproc://@/florian";

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
                int rv = mlm_client_connect (client, endpoint, 1000, "upses");
                assert (rv == 0);
                rv = mlm_client_set_producer (client, "upses");
                assert (rv >= 0);
            }
            else
            if (streq (command, "SEND")) {
                zmsg_t *response = zmsg_new();
                assert(response);
                int rv = zmsg_addstr(response, "Karol");
                assert (rv == 0);

                mlm_client_send (client, "", &response);

                zmsg_destroy (&response);
            }
            else {
            }

            zstr_free (&command);
            zmsg_destroy (&message);

            
        }

    }
    zpoller_destroy (&poller);
    mlm_client_destroy (&client);
}


int main (int argc, char **argv) {

    zactor_t *publisher = zactor_new (publisher_actor, NULL);
    assert (publisher);

    zactor_t *server = zactor_new (mlm_server, "malamute");
    assert (server);

    //zstr_send (server, "VERBOSE");
    zstr_sendx (server, "BIND", endpoint, NULL);
    zstr_sendx (server, "SET", "server/timeout", "10000", NULL);    

    zclock_sleep (500);

    mlm_client_t *listener = mlm_client_new ();
    assert (listener);

    mlm_client_connect (listener, endpoint, 1000, "listener");
    zclock_sleep (100);
    mlm_client_set_consumer (listener, "upses", ".*");


    zstr_sendx (publisher, "ENDPOINT", endpoint, NULL);
    zclock_sleep (100);

    zstr_sendx (publisher, "SEND", "upses", "Karol", NULL);

    printf("before recv\n");
    zmsg_t *msg = mlm_client_recv (listener);
    printf("message received\n");

    char *something =  zmsg_popstr (msg);
    assert (streq (something, "2Karol"));

    zstr_free (&something);

    zactor_destroy (&publisher);
    return EXIT_SUCCESS;

    //  Test 1
    {
    }
    //  Test 2
    {
    }
}
