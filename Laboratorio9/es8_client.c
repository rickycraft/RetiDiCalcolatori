#include <stdio.h>

#include "operazioni.h"
#define BUFF_DIM 100

int main(int argc, char *argv[]) {
  CLIENT *cl;
  char *server, *buff1, *buff2;
  int soglia;
  buff1 = malloc(BUFF_DIM * sizeof(char));
  buff2 = malloc(BUFF_DIM * sizeof(char));
  char c[10];
  if (argc < 2) {
    exit(1);
  }
  server = argv[1];
  cl = clnt_create(server, OPERAZIONIPROG, OPERAZIONIVERS, "udp");
  if (cl == NULL) {
    clnt_pcreateerror(server);
    exit(1);
  }

  printf("che programma vuoi usare?\nfile_scan (f) | dir_scan (d)\n");
  while (gets(c)) {
    if (c[0] == 'f') {
      // file scan
      printf("Digita il nome del file: \n");
      gets(buff1);
      Wc *wc = file_scan_1(&buff1, cl);
      if (wc == NULL) {
        fprintf(stderr, "%s: %s fallisce la rpc n\n", argv[0], server);
        clnt_perror(cl, server);
      } else if (wc->caratteri == -1) {
        printf("Non sono riuscito a trovare il file\n");
      } else {
        printf("l: %d, p: %d, c: %d\n", wc->righe, wc->parole, wc->caratteri);
      }
    } else if (c[0] == 'd') {
      // dir scan
      printf("Digita il nome della directory: \n");
      gets(buff1);
      printf("Digita la soglia minima: \n");
      gets(buff2);
      soglia = atoi(buff2);
      if (soglia == 0) {
        printf("soglia non è un intero, uso 0 come default\n");
      }
      NameSize namesize = {buff1, soglia};
      int *count = dir_scan_1(&namesize, cl);
      if (count == NULL) {
        fprintf(stderr, "%s: %s fallisce la rpc n\n", argv[0], server);
        clnt_perror(cl, server);
      } else {
        printf("n_file sopra soglia: %d\n", *count);
      }
    }
    printf("che programma vuoi usare?\nfile_scan (f) | dir_scan (d)\n");
  }
  free(buff1);
  free(buff2);
  clnt_destroy(cl);
}