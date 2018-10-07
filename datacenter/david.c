#include <malamute.h>

// build: gcc -lmlm -lczmq -g  david.c -o david 
//20181006 - david hauri

// serverpart by Karol
static const char *ENDPOINT = "tcp://192.168.1.164:9999";
static const char *STREAM = "upses";
static const char *CLIENTIDENTIFICATION = "david";

static const char *SIGNAL_ON = "ON";
static const char *SIGNAL_OFF = "OFF";

int main () {
    mlm_client_t *client = mlm_client_new();
    assert(client);

    int rv = mlm_client_connect(client, ENDPOINT, 2000, CLIENTIDENTIFICATION);
    assert(rv >= 0);

    srandom ((unsigned) time (NULL));

    uint8_t device_a_count = 0; 
    uint8_t device_b_count = 0; 
    uint8_t device_c_count = 0;

    uint8_t da_sleep = (uint8_t) randof (6);
    uint8_t db_sleep = (uint8_t) randof (6);
    uint8_t dc_sleep = (uint8_t) randof (6);

    while(!zsys_interrupted)
    {
        if (device_a_count >= da_sleep) {
            zmsg_t *message = zmsg_new ();
            assert (message);

            zmsg_addstr (message, "dvc_a");
            if (randof (2) == 0)
                zmsg_addstr (message, "ON");
            else
                zmsg_addstr (message, "OFF");

            int rv = mlm_client_send (client, "Hello", &message);
            assert (rv == 0);
            device_a_count = 0;
            da_sleep = randof (6);
        }      
        if (device_b_count >= da_sleep) {
            zmsg_t *message = zmsg_new ();
            assert (message);

            zmsg_addstr (message, "dvc_b");
            if (randof (2) == 0)
                zmsg_addstr (message, "ON");
            else
                zmsg_addstr (message, "OFF");

            int rv = mlm_client_send (client, "Hello", &message);
            assert (rv == 0);
            device_b_count = 0;
            da_sleep = randof (6);
        }      
        if (device_c_count >= dc_sleep) {
            zmsg_t *message = zmsg_new ();
            assert (message);

            zmsg_addstr (message, "dvc_c");
            if (randof (2) == 0)
                zmsg_addstr (message, "ON");
            else
                zmsg_addstr (message, "OFF");

            int rv = mlm_client_send (client, "Hello", &message);
            assert (rv == 0);
            device_c_count = 0;
            da_sleep = randof (6);
        }      


        device_a_count++;
        device_b_count++;
        device_c_count++;

        zclock_sleep (1000);
    }

    mlm_client_destroy(&client);

    return EXIT_SUCCESS;
}


