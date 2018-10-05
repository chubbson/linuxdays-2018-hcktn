/*  =========================================================================
    hello - hello example protocol

    Codec class for hello.

    ** WARNING *************************************************************
    THIS SOURCE FILE IS 100% GENERATED. If you edit this file, you will lose
    your changes at the next build cycle. This is great for temporary printf
    statements. DO NOT MAKE ANY CHANGES YOU WISH TO KEEP. The correct places
    for commits are:

     * The XML model used for this code generation: hello.xml, or
     * The code generation script that built this file: zproto_codec_c
    ************************************************************************
    =========================================================================
*/

/*
@header
    hello - hello example protocol
@discuss
@end
*/

#ifdef NDEBUG
#undef NDEBUG
#endif

#include "rfc_hello.h"
#include "../include/hello.h"

//  Structure of our class

struct _hello_t {
    zframe_t *routing_id;               //  Routing_id from ROUTER, if any
    int id;                             //  hello message ID
    byte *needle;                       //  Read/write pointer for serialization
    byte *ceiling;                      //  Valid upper limit for read pointer
    char reason [256];                  //  reason
};

//  --------------------------------------------------------------------------
//  Network data encoding macros

//  Put a block of octets to the frame
#define PUT_OCTETS(host,size) { \
    memcpy (self->needle, (host), size); \
    self->needle += size; \
}

//  Get a block of octets from the frame
#define GET_OCTETS(host,size) { \
    if (self->needle + size > self->ceiling) { \
        zsys_warning ("hello: GET_OCTETS failed"); \
        goto malformed; \
    } \
    memcpy ((host), self->needle, size); \
    self->needle += size; \
}

//  Put a 1-byte number to the frame
#define PUT_NUMBER1(host) { \
    *(byte *) self->needle = (byte) (host); \
    self->needle++; \
}

//  Put a 2-byte number to the frame
#define PUT_NUMBER2(host) { \
    self->needle [0] = (byte) (((host) >> 8)  & 255); \
    self->needle [1] = (byte) (((host))       & 255); \
    self->needle += 2; \
}

//  Put a 4-byte number to the frame
#define PUT_NUMBER4(host) { \
    self->needle [0] = (byte) (((host) >> 24) & 255); \
    self->needle [1] = (byte) (((host) >> 16) & 255); \
    self->needle [2] = (byte) (((host) >> 8)  & 255); \
    self->needle [3] = (byte) (((host))       & 255); \
    self->needle += 4; \
}

//  Put a 8-byte number to the frame
#define PUT_NUMBER8(host) { \
    self->needle [0] = (byte) (((host) >> 56) & 255); \
    self->needle [1] = (byte) (((host) >> 48) & 255); \
    self->needle [2] = (byte) (((host) >> 40) & 255); \
    self->needle [3] = (byte) (((host) >> 32) & 255); \
    self->needle [4] = (byte) (((host) >> 24) & 255); \
    self->needle [5] = (byte) (((host) >> 16) & 255); \
    self->needle [6] = (byte) (((host) >> 8)  & 255); \
    self->needle [7] = (byte) (((host))       & 255); \
    self->needle += 8; \
}

//  Get a 1-byte number from the frame
#define GET_NUMBER1(host) { \
    if (self->needle + 1 > self->ceiling) { \
        zsys_warning ("hello: GET_NUMBER1 failed"); \
        goto malformed; \
    } \
    (host) = *(byte *) self->needle; \
    self->needle++; \
}

//  Get a 2-byte number from the frame
#define GET_NUMBER2(host) { \
    if (self->needle + 2 > self->ceiling) { \
        zsys_warning ("hello: GET_NUMBER2 failed"); \
        goto malformed; \
    } \
    (host) = ((uint16_t) (self->needle [0]) << 8) \
           +  (uint16_t) (self->needle [1]); \
    self->needle += 2; \
}

//  Get a 4-byte number from the frame
#define GET_NUMBER4(host) { \
    if (self->needle + 4 > self->ceiling) { \
        zsys_warning ("hello: GET_NUMBER4 failed"); \
        goto malformed; \
    } \
    (host) = ((uint32_t) (self->needle [0]) << 24) \
           + ((uint32_t) (self->needle [1]) << 16) \
           + ((uint32_t) (self->needle [2]) << 8) \
           +  (uint32_t) (self->needle [3]); \
    self->needle += 4; \
}

//  Get a 8-byte number from the frame
#define GET_NUMBER8(host) { \
    if (self->needle + 8 > self->ceiling) { \
        zsys_warning ("hello: GET_NUMBER8 failed"); \
        goto malformed; \
    } \
    (host) = ((uint64_t) (self->needle [0]) << 56) \
           + ((uint64_t) (self->needle [1]) << 48) \
           + ((uint64_t) (self->needle [2]) << 40) \
           + ((uint64_t) (self->needle [3]) << 32) \
           + ((uint64_t) (self->needle [4]) << 24) \
           + ((uint64_t) (self->needle [5]) << 16) \
           + ((uint64_t) (self->needle [6]) << 8) \
           +  (uint64_t) (self->needle [7]); \
    self->needle += 8; \
}

//  Put a string to the frame
#define PUT_STRING(host) { \
    size_t string_size = strlen (host); \
    PUT_NUMBER1 (string_size); \
    memcpy (self->needle, (host), string_size); \
    self->needle += string_size; \
}

//  Get a string from the frame
#define GET_STRING(host) { \
    size_t string_size; \
    GET_NUMBER1 (string_size); \
    if (self->needle + string_size > (self->ceiling)) { \
        zsys_warning ("hello: GET_STRING failed"); \
        goto malformed; \
    } \
    memcpy ((host), self->needle, string_size); \
    (host) [string_size] = 0; \
    self->needle += string_size; \
}

//  Put a long string to the frame
#define PUT_LONGSTR(host) { \
    size_t string_size = strlen (host); \
    PUT_NUMBER4 (string_size); \
    memcpy (self->needle, (host), string_size); \
    self->needle += string_size; \
}

//  Get a long string from the frame
#define GET_LONGSTR(host) { \
    size_t string_size; \
    GET_NUMBER4 (string_size); \
    if (self->needle + string_size > (self->ceiling)) { \
        zsys_warning ("hello: GET_LONGSTR failed"); \
        goto malformed; \
    } \
    free ((host)); \
    (host) = (char *) malloc (string_size + 1); \
    memcpy ((host), self->needle, string_size); \
    (host) [string_size] = 0; \
    self->needle += string_size; \
}

//  --------------------------------------------------------------------------
//  bytes cstring conversion macros

// create new array of unsigned char from properly encoded string
// len of resulting array is strlen (str) / 2
// caller is responsibe for freeing up the memory
#define BYTES_FROM_STR(dst, _str) { \
    char *str = (char*) (_str); \
    if (!str || (strlen (str) % 2) != 0) \
        return NULL; \
\
    size_t strlen_2 = strlen (str) / 2; \
    byte *mem = (byte*) zmalloc (strlen_2); \
    size_t i; \
\
    for (i = 0; i != strlen_2; i++) \
    { \
        char buff[3] = {0x0, 0x0, 0x0}; \
        strncpy (buff, str, 2); \
        unsigned int uint; \
        sscanf (buff, "%x", &uint); \
        assert (uint <= 0xff); \
        mem [i] = (byte) (0xff & uint); \
        str += 2; \
    } \
    dst = mem; \
}

// convert len bytes to hex string
// caller is responsibe for freeing up the memory
#define STR_FROM_BYTES(dst, _mem, _len) { \
    byte *mem = (byte*) (_mem); \
    size_t len = (size_t) (_len); \
    char* ret = (char*) zmalloc (2*len + 1); \
    char* aux = ret; \
    size_t i; \
    for (i = 0; i != len; i++) \
    { \
        sprintf (aux, "%02x", mem [i]); \
        aux+=2; \
    } \
    dst = ret; \
}

//  --------------------------------------------------------------------------
//  Create a new hello

hello_t *
hello_new (void)
{
    hello_t *self = (hello_t *) zmalloc (sizeof (hello_t));
    return self;
}

//  --------------------------------------------------------------------------
//  Create a new hello from zpl/zconfig_t *

hello_t *
    hello_new_zpl (zconfig_t *config)
{
    assert (config);
    hello_t *self = NULL;
    char *message = zconfig_get (config, "message", NULL);
    if (!message) {
        zsys_error ("Can't find 'message' section");
        return NULL;
    }

    if (streq ("HELLO_OK", message)) {
        self = hello_new ();
        hello_set_id (self, HELLO_OK);
    }
    else
    if (streq ("HELLO_ERROR", message)) {
        self = hello_new ();
        hello_set_id (self, HELLO_ERROR);
    }
    else
    if (streq ("HELLO_HELLO", message)) {
        self = hello_new ();
        hello_set_id (self, HELLO_HELLO);
    }
    else
       {
        zsys_error ("message=%s is not known", message);
        return NULL;
       }

    char *s = zconfig_get (config, "routing_id", NULL);
    if (s) {
        byte *bvalue;
        BYTES_FROM_STR (bvalue, s);
        if (!bvalue) {
            hello_destroy (&self);
            return NULL;
        }
        zframe_t *frame = zframe_new (bvalue, strlen (s) / 2);
        free (bvalue);
        self->routing_id = frame;
    }

    zconfig_t *content = NULL;
    switch (self->id) {
        case HELLO_OK:
            break;
        case HELLO_ERROR:
            content = zconfig_locate (config, "content");
            if (!content) {
                zsys_error ("Can't find 'content' section");
                hello_destroy (&self);
                return NULL;
            }
            {
            char *s = zconfig_get (content, "reason", NULL);
            if (!s) {
                hello_destroy (&self);
                return NULL;
            }
            strncpy (self->reason, s, 256);
            }
            break;
        case HELLO_HELLO:
            break;
    }
    return self;
}


//  --------------------------------------------------------------------------
//  Destroy the hello

void
hello_destroy (hello_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        hello_t *self = *self_p;

        //  Free class properties
        zframe_destroy (&self->routing_id);

        //  Free object itself
        free (self);
        *self_p = NULL;
    }
}


//  --------------------------------------------------------------------------
//  Create a deep copy of a hello instance

hello_t *
hello_dup (hello_t *other)
{
    assert (other);
    hello_t *copy = hello_new ();

    // Copy the routing and message id
    hello_set_routing_id (copy, hello_routing_id (other));
    hello_set_id (copy, hello_id (other));

    // Copy the rest of the fields
    hello_set_reason (copy, hello_reason (other));

    return copy;
}

//  --------------------------------------------------------------------------
//  Receive a hello from the socket. Returns 0 if OK, -1 if
//  the recv was interrupted, or -2 if the message is malformed.
//  Blocks if there is no message waiting.

int
hello_recv (hello_t *self, zsock_t *input)
{
    assert (input);
    int rc = 0;
    zmq_msg_t frame;
    zmq_msg_init (&frame);

    if (zsock_type (input) == ZMQ_ROUTER) {
        zframe_destroy (&self->routing_id);
        self->routing_id = zframe_recv (input);
        if (!self->routing_id || !zsock_rcvmore (input)) {
            zsys_warning ("hello: no routing ID");
            rc = -1;            //  Interrupted
            goto malformed;
        }
    }
    int size;
    size = zmq_msg_recv (&frame, zsock_resolve (input), 0);
    if (size == -1) {
        zsys_warning ("hello: interrupted");
        rc = -1;                //  Interrupted
        goto malformed;
    }
    //  Get and check protocol signature
    self->needle = (byte *) zmq_msg_data (&frame);
    self->ceiling = self->needle + zmq_msg_size (&frame);

    uint16_t signature;
    GET_NUMBER2 (signature);
    if (signature != (0xAAA0 | 0)) {
        zsys_warning ("hello: invalid signature");
        rc = -2;                //  Malformed
        goto malformed;
    }
    //  Get message id and parse per message type
    GET_NUMBER1 (self->id);

    switch (self->id) {
        case HELLO_OK:
            break;

        case HELLO_ERROR:
            GET_STRING (self->reason);
            break;

        case HELLO_HELLO:
            break;

        default:
            zsys_warning ("hello: bad message ID");
            rc = -2;            //  Malformed
            goto malformed;
    }
    //  Successful return
    zmq_msg_close (&frame);
    return rc;

    //  Error returns
    malformed:
        zmq_msg_close (&frame);
        return rc;              //  Invalid message
}


//  --------------------------------------------------------------------------
//  Send the hello to the socket. Does not destroy it. Returns 0 if
//  OK, else -1.

int
hello_send (hello_t *self, zsock_t *output)
{
    assert (self);
    assert (output);

    if (zsock_type (output) == ZMQ_ROUTER)
        zframe_send (&self->routing_id, output, ZFRAME_MORE + ZFRAME_REUSE);

    size_t frame_size = 2 + 1;          //  Signature and message ID
    switch (self->id) {
        case HELLO_ERROR:
            frame_size += 1 + strlen (self->reason);
            break;
    }
    //  Now serialize message into the frame
    zmq_msg_t frame;
    zmq_msg_init_size (&frame, frame_size);
    self->needle = (byte *) zmq_msg_data (&frame);
    PUT_NUMBER2 (0xAAA0 | 0);
    PUT_NUMBER1 (self->id);
    size_t nbr_frames = 1;              //  Total number of frames to send

    switch (self->id) {
        case HELLO_ERROR:
            PUT_STRING (self->reason);
            break;

    }
    //  Now send the data frame
    zmq_msg_send (&frame, zsock_resolve (output), --nbr_frames? ZMQ_SNDMORE: 0);

    return 0;
}


//  --------------------------------------------------------------------------
//  Print contents of message to stdout

void
hello_print (hello_t *self)
{
    assert (self);
    switch (self->id) {
        case HELLO_OK:
            zsys_debug ("HELLO_OK:");
            break;

        case HELLO_ERROR:
            zsys_debug ("HELLO_ERROR:");
            zsys_debug ("    reason='%s'", self->reason);
            break;

        case HELLO_HELLO:
            zsys_debug ("HELLO_HELLO:");
            break;

    }
}

//  --------------------------------------------------------------------------
//  Export class as zconfig_t*. Caller is responsibe for destroying the instance

zconfig_t *
hello_zpl (hello_t *self, zconfig_t *parent)
{
    assert (self);

    zconfig_t *root = zconfig_new ("hello", parent);

    switch (self->id) {
        case HELLO_OK:
        {
            zconfig_put (root, "message", "HELLO_OK");

            if (self->routing_id) {
                char *hex = NULL;
                STR_FROM_BYTES (hex, zframe_data (self->routing_id), zframe_size (self->routing_id));
                zconfig_putf (root, "routing_id", "%s", hex);
                zstr_free (&hex);
            }

            break;
            }
        case HELLO_ERROR:
        {
            zconfig_put (root, "message", "HELLO_ERROR");

            if (self->routing_id) {
                char *hex = NULL;
                STR_FROM_BYTES (hex, zframe_data (self->routing_id), zframe_size (self->routing_id));
                zconfig_putf (root, "routing_id", "%s", hex);
                zstr_free (&hex);
            }

            zconfig_t *config = zconfig_new ("content", root);
            zconfig_putf (config, "reason", "%s", self->reason);
            break;
            }
        case HELLO_HELLO:
        {
            zconfig_put (root, "message", "HELLO_HELLO");

            if (self->routing_id) {
                char *hex = NULL;
                STR_FROM_BYTES (hex, zframe_data (self->routing_id), zframe_size (self->routing_id));
                zconfig_putf (root, "routing_id", "%s", hex);
                zstr_free (&hex);
            }

            break;
            }
    }
    return root;
}

//  --------------------------------------------------------------------------
//  Get/set the message routing_id

zframe_t *
hello_routing_id (hello_t *self)
{
    assert (self);
    return self->routing_id;
}

void
hello_set_routing_id (hello_t *self, zframe_t *routing_id)
{
    if (self->routing_id)
        zframe_destroy (&self->routing_id);
    self->routing_id = zframe_dup (routing_id);
}


//  --------------------------------------------------------------------------
//  Get/set the hello id

int
hello_id (hello_t *self)
{
    assert (self);
    return self->id;
}

void
hello_set_id (hello_t *self, int id)
{
    self->id = id;
}

//  --------------------------------------------------------------------------
//  Return a printable command string

const char *
hello_command (hello_t *self)
{
    assert (self);
    switch (self->id) {
        case HELLO_OK:
            return ("OK");
            break;
        case HELLO_ERROR:
            return ("ERROR");
            break;
        case HELLO_HELLO:
            return ("HELLO");
            break;
    }
    return "?";
}

//  --------------------------------------------------------------------------
//  Get/set the reason field

const char *
hello_reason (hello_t *self)
{
    assert (self);
    return self->reason;
}

void
hello_set_reason (hello_t *self, const char *value)
{
    assert (self);
    assert (value);
    if (value == self->reason)
        return;
    strncpy (self->reason, value, 255);
    self->reason [255] = 0;
}



//  --------------------------------------------------------------------------
//  Selftest

void
hello_test (bool verbose)
{
    printf (" * hello: ");

    if (verbose)
        printf ("\n");

    //  @selftest
    //  Simple create/destroy test
    zconfig_t *config;
    hello_t *self = hello_new ();
    assert (self);
    hello_destroy (&self);
    //  Create pair of sockets we can send through
    //  We must bind before connect if we wish to remain compatible with ZeroMQ < v4
    zsock_t *output = zsock_new (ZMQ_DEALER);
    assert (output);
    int rc = zsock_bind (output, "inproc://selftest-hello");
    assert (rc == 0);

    zsock_t *input = zsock_new (ZMQ_ROUTER);
    assert (input);
    rc = zsock_connect (input, "inproc://selftest-hello");
    assert (rc == 0);


    //  Encode/send/decode and verify each message type
    int instance;
    self = hello_new ();
    hello_set_id (self, HELLO_OK);

    // convert to zpl
    config = hello_zpl (self, NULL);
    if (verbose)
        zconfig_print (config);
    //  Send twice
    hello_send (self, output);
    hello_send (self, output);

    for (instance = 0; instance < 3; instance++) {
        hello_t *self_temp = self;
        if (instance < 2)
            hello_recv (self, input);
        else {
            self = hello_new_zpl (config);
            assert (self);
            zconfig_destroy (&config);
        }
        if (instance < 2)
            assert (hello_routing_id (self));
        if (instance == 2) {
            hello_destroy (&self);
            self = self_temp;
        }
    }
    hello_set_id (self, HELLO_ERROR);

    hello_set_reason (self, "Life is short but Now lasts for ever");
    // convert to zpl
    config = hello_zpl (self, NULL);
    if (verbose)
        zconfig_print (config);
    //  Send twice
    hello_send (self, output);
    hello_send (self, output);

    for (instance = 0; instance < 3; instance++) {
        hello_t *self_temp = self;
        if (instance < 2)
            hello_recv (self, input);
        else {
            self = hello_new_zpl (config);
            assert (self);
            zconfig_destroy (&config);
        }
        if (instance < 2)
            assert (hello_routing_id (self));
        assert (streq (hello_reason (self), "Life is short but Now lasts for ever"));
        if (instance == 2) {
            hello_destroy (&self);
            self = self_temp;
        }
    }
    hello_set_id (self, HELLO_HELLO);

    // convert to zpl
    config = hello_zpl (self, NULL);
    if (verbose)
        zconfig_print (config);
    //  Send twice
    hello_send (self, output);
    hello_send (self, output);

    for (instance = 0; instance < 3; instance++) {
        hello_t *self_temp = self;
        if (instance < 2)
            hello_recv (self, input);
        else {
            self = hello_new_zpl (config);
            assert (self);
            zconfig_destroy (&config);
        }
        if (instance < 2)
            assert (hello_routing_id (self));
        if (instance == 2) {
            hello_destroy (&self);
            self = self_temp;
        }
    }

    hello_destroy (&self);
    zsock_destroy (&input);
    zsock_destroy (&output);
#if defined (__WINDOWS__)
    zsys_shutdown();
#endif
    //  @end

    printf ("OK\n");
}
