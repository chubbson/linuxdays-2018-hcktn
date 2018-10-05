#!/bin/bash
rm -r rfc-hello-user
gcc -lczmq rfc-hello-user.c -o rfc-hello-user
gcc -Wall provider.c -o provider -L/usr/local/lib -lzmq -lczmq

