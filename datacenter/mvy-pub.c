#include <malamute.h>

// build: gcc -lmlm -lczmq mvy-pub.c -o mvy-pub

static const char *ENDPOINT = "tcp://192.168.1.164:9999";
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

static void
s_pub_actor (zsock_t *pipe, void* args)
{
    pub_t *self = pub_new (pipe);
    zsock_signal (pipe, 0);

    while (!zsys_interrupted)
    {
        void *sock = pub_wait (self);

        if (sock == pipe) {
            char *endpoint;
            char *name;
            char *upsname;
            zstr_recvx (pipe, &endpoint, &name, &upsname, NULL);
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
        else {
            pub_send (self);
        }
    }

    pub_destroy (&self);

}

int main () {

    zactor_t *a1 = zactor_new (s_pub_actor, NULL);
    zstr_sendx (a1, ENDPOINT, "mvy-pub-1", "mvy-ups-1", NULL);
    zactor_t *a2 = zactor_new (s_pub_actor, NULL);
    zstr_sendx (a2, ENDPOINT, "mvy-pub-2", "mvy-ups-2", NULL);
    zactor_t *a3 = zactor_new (s_pub_actor, NULL);
    zstr_sendx (a3, ENDPOINT, "mvy-pub-3", "mvy-ups-3", NULL);

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

}
