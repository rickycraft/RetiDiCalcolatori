#!/bin/bash

YELLOW='\033[1;33m'
NOCOLOR='\033[0m'
CLASSPATH=$(pwd)

echo -e "${YELLOW}setup## Compile ${NOCOLOR}"
rm bin/*.class 2>/dev/null
javac -d bin *.java

echo -e "${YELLOW}setup## Generate stub ${NOCOLOR}"
cd bin
rmic -vcompat Server

echo -e "${YELLOW}setup## Start server${NOCOLOR}"
rmiregistry &
java Server &
sleep 1

echo -e "${YELLOW}setup## Start client${NOCOLOR}"
java Client 127.0.0.1 Server

echo ""
echo -e "${YELLOW}setup## Cleanup${NOCOLOR}"
kill %2
kill %1
