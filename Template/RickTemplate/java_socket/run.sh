#!/bin/bash

CLASSPATH=$(pwd):$(pwd)/bin
YELLOW='\033[1;33m'
NOCOLOR='\033[0m'

echo -e "${YELLOW}setup## Compile .class${NOCOLOR}"
rm bin/*.class 2>/dev/null
javac -d bin *.java
cd bin

# just compile
if [ "$1" == "-c" ]; then
  exit 0
fi

if [ "$1" == "udp" ]; then
  echo -e "${YELLOW}setup## Start the udp server${NOCOLOR}"
  java ServerUdp 8001 &
  sleep 1
  echo -e "${YELLOW}setup## Start the udp client${NOCOLOR}"
  java ClientUdp 127.0.0.1 8001
else
  echo -e "${YELLOW}setup## Start the tcp server${NOCOLOR}"
  java ServerTcp 8001 &
  sleep 1
  echo -e "${YELLOW}setup## Start the tcp client${NOCOLOR}"
  java ClientTcp 127.0.0.1 8001
fi

sleep 1
echo ""
echo -e "${YELLOW}setup## Cleanup${NOCOLOR}"
kill %1
