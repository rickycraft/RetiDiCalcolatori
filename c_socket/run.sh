#!/bin/bash

# compile
gcc client_tcp.c -Wunused -o client_tcp.bin
gcc client_udp.c -Wunused -o client_udp.bin
gcc server.c -Wunused -o server.bin

# run
echo "#### server ####"
./server.bin 5050 &
sleep 1
if [ "$1" == "udp" ]; then
  echo "#### udp client ####"
  ./client_udp.bin 127.0.0.1 5050
else
  echo "#### tcp client ####"
  ./client_tcp.bin 127.0.0.1 5050
fi
sleep 1

# clean
kill %1
kill %1
