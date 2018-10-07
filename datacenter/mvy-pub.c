#include <malamute.h>

// build: gcc -lmlm -lczmq mvy-pub.c -o mvy-pub

static const char *STREAM = "upses";

typedef struct _pub_t {
    char *endpoint;
    char *name;
    char *upsname;
    zpoller_t *poller;
    mlm_client_t *client;
    int interval;
} pub_t;

pub_t*
pub_new (zsock_t *pipe)
{
    pub_t *self = (pub_t*) zmalloc (sizeof (pub_t));
    assert (self);
    self->interval = -1;
    self->poller = zpoller_new (pipe, NULL);
    assert (self->poller);
    return self;
}

void
pub_destroy (pub_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        pub_t *self = *self_p;
        zstr_free (&self->endpoint);
        zstr_free (&self->name);
        zstr_free (&self->upsname);
        zpoller_destroy (&self->poller);
        mlm_client_destroy (&self->client);
        free(self);
        *self_p = NULL;
    }
}

void
pub_set_endpoint (pub_t *self, const char *endpoint)
{
    assert (self);
    assert (endpoint);
    zstr_free (&self->endpoint);
    self->endpoint = strdup (endpoint);
}

const char*
pub_get_endpoint (pub_t *self)
{
    assert (self);
    return (const char*) self->endpoint;
}

void
pub_set_name (pub_t *self, const char *name)
{
    assert (self);
    assert (name);
    zstr_free (&self->name);
    self->name = strdup (name);
}

const char*
pub_get_name (pub_t *self)
{
    assert (self);
    return (const char*) self->name;
}

void
pub_set_upsname (pub_t *self, const char *upsname)
{
    assert (self);
    assert (upsname);
    zstr_free (&self->upsname);
    self->upsname = strdup (upsname);
}

// connect + subscribe
int
pub_connect (pub_t *self)
{
    assert (self);
    assert (!self->client);
    assert (self->endpoint);
    assert (self->name);
    assert (self->upsname);

    self->client = mlm_client_new ();
    assert (self->client);
    int r = mlm_client_connect (self->client, self->endpoint, 5000, self->name);
    if (r != 0) {
        zsys_error ("cannot connect to %s", self->endpoint);
        mlm_client_destroy (&self->client);
        return -1;
    }
    r = mlm_client_set_producer (self->client, STREAM);
    if (r != 0) {
        zsys_error ("cannot connect to STREAM %s", STREAM);
        mlm_client_destroy (&self->client);
        return -1;
    }
    zpoller_add (self->poller, mlm_client_msgpipe (self->client));
    self->interval = 1000 + (random () % 5000);
    return 0;
}

void *
pub_wait (pub_t *self)
{
    assert (self);
    return zpoller_wait (self->poller, self->interval);
}

void
pub_send (pub_t *self)
{
    assert (self);
    if (!self->client)
        return;
    bool is_on = random() >= (RAND_MAX / 2);
    mlm_client_sendx (self->client, self->name, self->name, is_on ? "ON" : "OFF", NULL);
}

bool
pub_is_mlm (pub_t *self, void *sock)
{
    assert (self);
    return sock == mlm_client_msgpipe (self->client);
}

// publishing actor
// Pipe commands:
//      CONNECT/$ENDPOINT:str/$CLIENT_NAME/$UPS_NAME    - connect to malamute $ENDPOINT as $CLIENT_NAME and publish data for $UPS_NAME
//      $TERM - ends the actor
static void
s_pub_actor (zsock_t *pipe, void* args)
{
    pub_t *self = pub_new (pipe);       // <- this runs in MAIN THREAD
    zsock_signal (pipe, 0);             // thread is started from here

    // THIS RUNS IN SEPARATE THREAD
    // vvvvv
    while (!zsys_interrupted)
    {
        void *sock = pub_wait (self);

        if (sock == pipe) {
            zmsg_t *msg = zmsg_recv (pipe);
            char *command = zmsg_popstr (msg);
            assert (command); // crash if there is nothing sent

            if (streq (command, "$TERM")) {
                zstr_free (&command);
                zmsg_destroy (&msg);
                break;
            }
            else
            if (streq (command, "CONNECT")) {
                char *endpoint = zmsg_popstr (msg);
                char *name = zmsg_popstr (msg);
                char *upsname = zmsg_popstr (msg);
                pub_set_endpoint (self, endpoint);
                pub_set_name (self, name);
                pub_set_upsname (self, upsname);
                int r = pub_connect (self);
                if (r != 0) {
                    zsys_error ("Connection to '%s' as '%s' failed", endpoint, name);
                }
                zstr_free (&endpoint);
                zstr_free (&name);
                zstr_free (&upsname);
            }
            else
                zsys_warning ("Uknown command %s", command);
            
            zstr_free (&command);
            zmsg_destroy (&msg);
        }
        else
        if (pub_is_mlm (self, sock)) {
            // TODO: Problem: we do not print anything
            // Solution: put the printing code here
            // mlm_client_recv ...
        }
        else {
            pub_send (self);
        }
    }

    pub_destroy (&self);

}

int main () {

    static const char *ENDPOINT = "inproc://test";
    zactor_t *server = zactor_new (mlm_server, "Malamute");
    zstr_sendx (server, "BIND", ENDPOINT, NULL);

    zactor_t *a1 = zactor_new (s_pub_actor, NULL);
    zstr_sendx (zactor_sock (a1), "CONNECT", ENDPOINT, "mvy-pub-1", "mvy-ups-1", NULL);
    zactor_t *a2 = zactor_new (s_pub_actor, NULL);
    zstr_sendx (a2, "CONNECT", ENDPOINT, "mvy-pub-2", "mvy-ups-2", NULL);
    zactor_t *a3 = zactor_new (s_pub_actor, NULL);
    zstr_sendx (a3, "CONNECT", ENDPOINT, "mvy-pub-3", "mvy-ups-3", NULL);

    while (true) {
        char *message = zstr_recv (a1);
        if (message) {
            puts (message);
            free (message);
        }
        else {
            puts ("interrupted");
            break;
        }
    }

    zactor_destroy (&a3);
    zactor_destroy (&a2);
    zactor_destroy (&a1);

    zactor_destroy (&server);

}
