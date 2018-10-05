/*  =========================================================================
    utils - Utils

    Copyright (c) the Authors
    =========================================================================
*/

#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

//  @interface
//  Create a new utils
RFC_HELLO_EXPORT char *
    utils_endpoint (void);

//  Self test of this class
RFC_HELLO_EXPORT void
    utils_test (bool verbose);

//  @end

#ifdef __cplusplus
}
#endif

#endif
