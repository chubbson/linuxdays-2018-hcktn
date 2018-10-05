/*  =========================================================================
    utils - Utils

    Copyright (c) the Authors
    =========================================================================
*/

/*
@header
    utils - Utils
@discuss
@end
*/

#include "rfc_hello_classes.h"

//  Structure of our class

struct _utils_t {
    int filler;     //  Declare class properties here
};


//  --------------------------------------------------------------------------
//  Create a new utils

char *
utils_endpoint (void)
{
    char *x = NULL;
    asprintf (&x, "%s", "tcp://10.0.24.");
    return x;
}


//  --------------------------------------------------------------------------
//  Self test of this class

// If your selftest reads SCMed fixture data, please keep it in
// src/selftest-ro; if your test creates filesystem objects, please
// do so under src/selftest-rw.
// The following pattern is suggested for C selftest code:
//    char *filename = NULL;
//    filename = zsys_sprintf ("%s/%s", SELFTEST_DIR_RO, "mytemplate.file");
//    assert (filename);
//    ... use the "filename" for I/O ...
//    zstr_free (&filename);
// This way the same "filename" variable can be reused for many subtests.
#define SELFTEST_DIR_RO "src/selftest-ro"
#define SELFTEST_DIR_RW "src/selftest-rw"

void
utils_test (bool verbose)
{
    printf (" * utils: ");

    //  @selftest
    //  Simple create/destroy test

    char *y = utils_endpoint();
    assert(y);
    zstr_free(&y);
    printf ("OK\n");
}
