#!/bin/bash
rm -f karol
gcc -lczmq -lmlm karol.c -o karol
gcc -lmlm -lczmq intergration_test.c -g -o integration_test
