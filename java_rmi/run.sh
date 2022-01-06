#!/bin/bash

CLASSPATH=$(pwd)

echo setup#\# Compile .class
rm bin/*.class 2>/dev/null
javac -d bin *.java

echo setup#\# Generate stub/skeleton
cd bin
rmic -vcompat Server

echo setup#\# Start the server
rmiregistry &
java Server &
sleep 1

echo setup#\# Start the client
java Client 127.0.0.1 Server

echo ""
echo setup#\# Cleanup
kill %2
kill %1
