#!/bin/bash
rm -f karol
gcc -lczmq -lmlm karol.c -o karol
