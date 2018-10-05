/*  =========================================================================
    baltree - balanced tree

    Copyright (c) the Authors
    =========================================================================
*/

/*
@header
    baltree - balanced tree
@discuss
@end
*/

#include "rfc_hello_classes.h"

//  Structure of our class

struct _baltree_t {
    int filler;     //  Declare class properties here
};


//  --------------------------------------------------------------------------
//  Create a new baltree

baltree_t *
baltree_new (void)
{
    baltree_t *self = (baltree_t *) zmalloc (sizeof (baltree_t));
    assert (self);
    //  Initialize class properties here
    return self;
}


//  --------------------------------------------------------------------------
//  Destroy the baltree

void
baltree_destroy (baltree_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        baltree_t *self = *self_p;
        //  Free class properties here
        //  Free object itself
        free (self);
        *self_p = NULL;
    }
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
baltree_test (bool verbose)
{
    printf (" * baltree: ");

    //  @selftest
    //  Simple create/destroy test
    baltree_t *self = baltree_new ();
    assert (self);
    baltree_destroy (&self);
    //  @end
    printf ("OK\n");
}
