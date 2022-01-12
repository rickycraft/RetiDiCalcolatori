
#include <rpc/rpc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "operation.h"

#define DIM 16

int main(int argc, char *argv[]) {
  char *host;  // nome host
  CLIENT *cl;  // gestore del trasporto
  char buffer[DIM];
  // dichiarazioni delle variabili
  void *voidValue;
  Dato *dato;
  int *res;

  // CONTROLLO DEGLI ARGOMENTI
  if (argc != 2) {
    printf("client# argc != 2\n");
    exit(1);
  }
  host = argv[1];

  // CREAZIONE GESTORE DI TRASPORTO
  cl = clnt_create(host, OPERATION, OPERATIONVERS, "udp");
  if (cl == NULL) {
    clnt_pcreateerror(host);
    exit(1);
  }
  printf("client# Creato il gestore di trasporto\n");

  // INTERAZIONE CON L UTENTE
  printf("client# Selezionare operazione 1(get)/2(set): ");
  while (gets(buffer)) {
    if (strcmp(buffer, "1") == 0) {
      // Invocazione remota
      dato = get_1(voidValue, cl);

      // Controllo del risultato
      if (dato == NULL) {
        clnt_perror(cl, host);
        printf("client# Rpc error\n");
        exit(1);
      }
      printf("client# Ricevuto %s\n", dato->contenuto);
    } else if (strcmp(buffer, "2") == 0) {
      // Invocazione remota
      printf("Inserisci contenuto: ");
      if (gets(buffer) == NULL) continue;
      if (strlen(buffer) >= MAX_STRING) {
        printf(
            "client# Stringa troppo lunga\n"
            "client# Selezionare operazione 1(get)/2(set): ");
        continue;
      }
      Dato d = {buffer};
      res = set_1(&d, cl);

      // Controllo del risultato
      if (res == NULL) {
        clnt_perror(cl, host);
        printf("client# Rpc error\n");
        exit(1);
      }

    } else {
      printf("client# Operazione richiesta non disponibile!!\n");
    }

    printf("client# Selezionare operazione 1(get)/2(set): ");
  }  // while

  // Libero le risorse, distruggendo il gestore di trasporto
  clnt_destroy(cl);
  printf("\nclient# Termino\n");
  exit(0);
}  // main
