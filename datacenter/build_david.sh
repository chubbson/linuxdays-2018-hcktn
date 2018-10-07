
rm -f david
rm -f david_monitor

gcc -lmlm -lczmq -g  david.c -o david 
gcc -lmlm -lczmq -g  david_monitor.c -o david_monitor 