#!/bin/bash

rm -f flo
gcc -Wall flo.c -o flo -L/usr/local/lib -lmlm  -lczmq
