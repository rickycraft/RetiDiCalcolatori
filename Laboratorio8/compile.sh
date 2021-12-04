#!/bin/sh
gcc es8_client.c operazioni_xdr.c operazioni_clnt.c -o es8_client.bin -g
gcc operazioni_xdr.c operazioni_svc.c es8_proc.c -o server.bin
