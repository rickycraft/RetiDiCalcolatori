#include <stdio.h>

#include "operazioni.h"

int main(int argc, char *argv[]) {
  CLIENT *cl;
  char *server;
  if (argc < 2) {
    exit(1);
  }
  server = argv[1];
  cl = clnt_create(server, OPERAZIONIPROG, OPERAZIONIVERS, "udp");
  if (cl == NULL) {
    clnt_pcreateerror(server);
    exit(1);
  }

  Wc *wc = file_scan_1(&argv[2], cl);
  NameSize namesize = {argv[3], atoi(argv[4])};
  printf("name: %s, size: %d\n", namesize.nome, namesize.dim);
  int *count = dir_scan_1(&namesize, cl);

  printf("l: %d, w: %d, c: %d, count: %d\n", wc->righe, wc->parole,
         wc->caratteri, *count);
}