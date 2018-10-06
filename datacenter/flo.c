#include <malamute.h>
#include <czmq.h>
#include <unistd.h>

int main(int argc, char *argv []) {
    const char *endpoint = "tcp://192.168.1.164:9999";
    mlm_client_t *client = mlm_client_new();
    assert (client);

    int rv = mlm_client_connect(client, endpoint, 2000, "flo");
    assert(rv);
    rv = mlm_client_set_producer(client, "upses");
    assert (rv >= 0);

    while (!zsys_interrupted) {
        rv = mlm_client_sendx (client, "flo_ups", "ON", NULL);
        assert (rv == 0);
        sleep (1);
    }

    mlm_client_destroy (&client);
}
