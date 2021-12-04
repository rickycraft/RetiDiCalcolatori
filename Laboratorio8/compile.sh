#!/bin/sh
gcc es8_client.c operazioni_xdr.c operazioni_clnt.c -o client.bin
gcc operazioni_xdr.c operazioni_svc.c es8_proc.c -o server.bin
