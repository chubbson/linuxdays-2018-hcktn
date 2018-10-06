#include <malamute.h>
#include <czmq.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

int get_random_seconds() {
    int n = rand() % (5 + 1) + 1;
    return n;
}
int main(int argc, char *argv []) {
    const char *endpoint = "tcp://192.168.1.164:9999";
    mlm_client_t *client = mlm_client_new();
    assert (client);

    int rv = mlm_client_connect(client, endpoint, 2000, "flo");
    assert(rv >= 0);
    rv = mlm_client_set_producer(client, "upses");
    assert (rv >= 0);

    time_t next_ups_event = time(NULL);
    time_t next_server_event = time(NULL);
    time_t next_security_event = time(NULL);

    const char* on_signal = "ON";
    const char* off_signal = "OFF";

    while (!zsys_interrupted) {
        if (difftime(time(NULL), next_ups_event) > 0) {
            printf("Send: flo_ups\n");
            if(get_random_seconds() % 2) {
                rv = mlm_client_sendx (client, "flosubject", "flo_ups", off_signal, NULL);
            } else {
                rv = mlm_client_sendx (client, "flosubject", "flo_ups", on_signal, NULL);
            }
            assert (rv == 0);
            next_ups_event = time(NULL) + get_random_seconds();
        }

        if (difftime(time(NULL), next_security_event) > 0) {
            printf("Send: flo_security\n");
            if(get_random_seconds() % 2) {
                rv = mlm_client_sendx (client, "flosubject", "flo_security", on_signal, NULL);
            } else {
                rv = mlm_client_sendx (client, "flosubject", "flo_security", off_signal, NULL);
            }
            assert (rv == 0);
            next_security_event = time(NULL) + get_random_seconds();
        }

        if (difftime(time(NULL), next_server_event) > 0) {
            if(get_random_seconds() % 2) {
                rv = mlm_client_sendx (client, "flosubject", "flo_server", on_signal, NULL);
            } else {
                rv = mlm_client_sendx (client, "flosubject", "flo_server", off_signal, NULL);
            }
            printf("Send: flo_server\n");
            assert (rv == 0);
            next_server_event = time(NULL) + get_random_seconds();
        }
    }

    mlm_client_destroy (&client);
}
