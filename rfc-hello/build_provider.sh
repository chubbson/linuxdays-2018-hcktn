#!/bin/sh
gcc -Wall provider.c -o provider -L/usr/local/lib -lzmq -lczmq
