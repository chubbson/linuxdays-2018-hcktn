#include <malamute.h>

// build: gcc -lmlm -lczmq -g  mvy-pubc -o mvy-pub 
//20181006 - david hauri

// serverpart by Karol
static const char *ENDPOINT = "tcp://192.168.1.164:9999";
static const char *STREAM = "upses";

static const char *SIGNAL_ON = "ON";
static const char *SIGNAL_OFF = "OFF";

int main () {
    mlm_client_t *client = mlm_client_new();
    assert(client);

    int rv = mlm_client_connect(client, ENDPOINT, 2000, "david");
    assert(rv >= 0);

    while(!zsys_interrupted)
    {
    
    }

    mlm_client_destroy(&client);
}

