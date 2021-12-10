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

  printf("che programma vuoi usare?\nclassifica_giudici (c) | esprimi_voto (e)\n");
  while (gets(c)) {
    if (c[0] == 'c') {
      // file scan

    } else if (c[0] == 'e') {
      // dir scan
      printf("Digita il nome del concorrente: \n");
      gets(buff1);
      printf("Vuoi aggiungere o togliere un voto? aggiungere(+) | togliere(-): \n");
      gets(buff2);
     if(buff2[0]=='+'){
       printf("Hai scelto di aggiungere un voto al concorrente %s\n",buff1);
     }
     if(buff2[0]=='-'){
       printf("Hai scelto di togliere un voto al concorrente %s\n",buff1);
     }

      CandidatoVoto candidato={buff1,buff2[0]};
      
      int *count = esprimi_voto_1(&candidato, cl);
      if (count == NULL) {
        fprintf(stderr, "%s: %s fallisce la rpc n\n", argv[0], server);
        clnt_perror(cl, server);
      } else {

        if(*count==0){
          printf("Il candidato non è stato trovato");
        }
        if(*count==1){
        printf("modifica del voto effettuata con successo!\n");  
        }
        
      }
    }
    printf("che programma vuoi usare?\nclassifica_giudici (c) | esprimi_voto (e)\n");
 
  }
  free(buff1);
  free(buff2);
  clnt_destroy(cl);
}