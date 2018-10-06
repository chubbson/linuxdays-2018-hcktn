#include <malamute.h>

// build: gcc -lmlm -lczmq mvy-pub.c -o mvy-pub

static const char *ENDPOINT = "tcp://192.168.1.164:9999";
static const char *STREAM = "upses";

int main () {

    mlm_client_t *client = mlm_client_new ();
    assert (client);
    int r = mlm_client_connect (client, ENDPOINT, 5000, "mvy-pub");
    assert (r == 0);
    r = mlm_client_set_producer (client, STREAM);
    assert (r == 0);

    bool is_on = true;

    while (!zsys_interrupted) {
        zmsg_t *msg = zmsg_new ();
        zmsg_addstr (msg, "UPC-MVY");
        zmsg_addstr (msg, is_on? "ON" : "OFF");
        is_on != is_on;
        mlm_client_send (client, "UPC-MVY", &msg);
        zclock_sleep (2000);
    }

    mlm_client_destroy (&client);

}
