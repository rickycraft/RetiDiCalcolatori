#!/bin/bash

YELLOW='\033[1;33m'
NOCOLOR='\033[0m'

echo -e "${YELLOW}setup## Compile ${NOCOLOR}"
gcc client_tcp.c -Wunused -g -o bin/client_tcp.bin
gcc client_udp.c -Wunused -g -o bin/client_udp.bin
gcc server.c -Wunused -o bin/server.bin
cd bin

# just compile
if [ "$1" == "-c" ]; then
  exit 0
fi

echo -e "${YELLOW}setup## Start select server${NOCOLOR}"
pkill server.bin
./server.bin 5050 &
sleep 1
if [ "$1" == "udp" ]; then
  echo -e "${YELLOW}setup## Start udp client${NOCOLOR}"
  ./client_udp.bin 127.0.0.1 5050
else
  echo -e "${YELLOW}setup## Start tcp client${NOCOLOR}"
  ./client_tcp.bin 127.0.0.1 5050
fi

echo ""
echo -e "${YELLOW}setup## Cleanup${NOCOLOR}"
kill %1
