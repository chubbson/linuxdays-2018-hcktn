/*  =========================================================================
    baltree - balanced tree

    Copyright (c) the Authors
    =========================================================================
*/

#ifndef BALTREE_H_INCLUDED
#define BALTREE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

//  @interface
//  Create a new baltree
RFC_HELLO_EXPORT baltree_t *
    baltree_new (void);

//  Destroy the baltree
RFC_HELLO_EXPORT void
    baltree_destroy (baltree_t **self_p);

//  Self test of this class
RFC_HELLO_EXPORT void
    baltree_test (bool verbose);

//  @end

#ifdef __cplusplus
}
#endif

#endif
