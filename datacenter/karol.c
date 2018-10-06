#include <malamute.h>

int
main (int argc, char **argv) {
    if (argc < 2) {
        printf ("USAGE: %s: <endpoint>\n", argv [0]);
        return EXIT_FAILURE;
    }

    const char *endpoint = argv [1];
    const char *subscribe_stream = argv [2];

    printf ("Endpoint: '%s'\n", endpoint);

    mlm_client_t *client = mlm_client_new ();
    assert (client);

    int rv = mlm_client_connect (client, endpoint, 1000, "datacenter-karol");
    assert (rv == 0);

    rv = mlm_client_set_producer (client, "upses");
    assert (rv == 0);

    uint8_t server_count = 0, server_sleep = (uint8_t) randof (6);
    uint8_t ups_count = 0, ups_sleep = (uint8_t) randof (6);
    uint8_t switch_count = 0, switch_sleep = (uint8_t) randof (6);

    while (!zsys_interrupted) {
        if (server_count >= server_sleep) {
            zmsg_t *message = zmsg_new ();
            assert (message);

            zmsg_addstr (message, "server");
            int chance = randof (2);
            if (chance == 0)
                zmsg_addstr (message, "ON");
            else
                zmsg_addstr (message, "OFF");

            int rv = mlm_client_send (client, "Hello", &message);
            assert (rv == 0);
            server_count = 0;
            server_sleep = randof (6);
        }
        if (ups_count >= ups_sleep) {
            zmsg_t *message = zmsg_new ();
            assert (message);

            zmsg_addstr (message, "ups");
            int chance = randof (2);
            if (chance == 0)
                zmsg_addstr (message, "ON");
            else
                zmsg_addstr (message, "OFF");

            int rv = mlm_client_send (client, "Hello", &message);
            assert (rv == 0);
            ups_count = 0;
            ups_sleep = randof (6);
        }
        if (switch_count >= switch_sleep) {
            zmsg_t *message = zmsg_new ();
            assert (message);

            zmsg_addstr (message, "switch");
            int chance = randof (2);
            if (chance == 0)
                zmsg_addstr (message, "ON");
            else
                zmsg_addstr (message, "OFF");

            int rv = mlm_client_send (client, "Hello", &message);
            assert (rv == 0);
            switch_count = 0;
            switch_sleep = randof (6);
        }

        server_count++;
        ups_count++;
        switch_count++;

        zclock_sleep (1000);
    }


    mlm_client_destroy (&client);

    return EXIT_SUCCESS;
}
