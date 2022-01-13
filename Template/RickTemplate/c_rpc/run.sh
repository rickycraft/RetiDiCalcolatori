#!/bin/bash

YELLOW='\033[1;33m'
NOCOLOR='\033[0m'

echo -e "${YELLOW}setup## Compile ${NOCOLOR}"
rpcgen operation.x
mv operation_* bin
cp operation.h bin
cd bin
gcc -o client.bin operation_xdr.c operation_clnt.c ../client.c
gcc -o server.bin operation_xdr.c operation_svc.c ../server.c

# just compile
if [ "$1" == "-c" ]; then
  exit 0
fi

echo -e "${YELLOW}setup## Start server${NOCOLOR}"
sudo rpcbind 2>/dev/null
./server.bin &
sleep 1
echo -e "${YELLOW}setup## Start client${NOCOLOR}"
./client.bin 127.0.0.1

sleep 1
echo ""
echo -e "${YELLOW}setup## Cleanup${NOCOLOR}"
kill %1
