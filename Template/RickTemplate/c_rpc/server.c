#include <rpc/rpc.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "operation.h"

static Dato s_dato;
static int inizializzato = false;

void init() {
  if (!inizializzato) {
    s_dato.contenuto = malloc(MAX_STRING * sizeof(char));
    strncpy(s_dato.contenuto, "stringa", MAX_STRING - 1);
    inizializzato = true;
  }
}

int *set_1_svc(Dato *dato, struct svc_req *cl) {
  init();
  static int esito;
  esito = -1;
  if (dato != NULL) {
    printf("server# set_1 Dato: %s, s_dato: %s\n", dato->contenuto,
           s_dato.contenuto);
    strncpy(s_dato.contenuto, dato->contenuto, MAX_STRING - 1);
    esito = 1;
  }
  return &esito;
}

Dato *get_1_svc(void *v, struct svc_req *cl) {
  init();
  printf("server# get_1 Dato: %s\n", s_dato.contenuto);
  return &s_dato;
}