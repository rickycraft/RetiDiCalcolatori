#include <stdio.h>

#include "operazioni.h"

#define BUFF_DIM 100

int main(int argc, char *argv[])
{
  CLIENT *cl;
  char *server, *buff1, *buff2;
  int soglia;
  void *in;
  Giudici *result_giudici;
  CandidatoVoto *candidato;
  int *result_espVoto;
  buff1 = malloc(BUFF_DIM * sizeof(char)); //per esprimi_voto
  buff2 = malloc(BUFF_DIM * sizeof(char)); //per esprimi_voto
  char c[256];
  if (argc != 2)
  {
    printf("usage: %s server_host\n", argv[0]);
    exit(1);
  }
  server = argv[1];
  cl = clnt_create(server, OPERAZIONIPROG, OPERAZIONIVERS, "udp");
  if (cl == NULL)
  {
    clnt_pcreateerror(server);
    exit(1);
  }
  //printf("[DEBUG] creato trasporto\n");

  printf("che programma vuoi usare?\nclassifica_giudici (c) | esprimi_voto (e)\n");
  while (gets(c))
  {
    if (c[0] == 'c')
    { //classifica Giudici
      result_giudici = classifica_giudici_1(in, cl);
      if (result_giudici == (Giudici *)NULL)
      {
        fprintf(stderr, "%s: %s fallisce la rpc in classifica_giudici \n", argv[0], server);
        clnt_perror(cl, server);
      }

      printf("Giudici (in ordine di voto)\n");
      for (int i = 0; i < NUMGIUDICI; i++)
      {

        if (strcmp(result_giudici->giudici_ordinati[i].nome, "L") != 0)
        {
          printf("nome:%s \tVoto:%d\n", (result_giudici->giudici_ordinati[i].nome),result_giudici->giudici_ordinati[i].votiTotali);
        }
      }
    }
    else if (c[0] == 'e')
    {
      printf("Digita il nome del concorrente: \n");
      gets(buff1);
      printf("Vuoi aggiungere o togliere un voto? aggiungere(+) | togliere(-): \n");
      gets(buff2);
      if (buff2[0] == '+')
      {
        printf("Hai scelto di aggiungere un voto al concorrente %s\n", buff1);
      }
      if (buff2[0] == '-')
      {
        printf("Hai scelto di togliere un voto al concorrente %s\n", buff1);
      }
      candidato=(CandidatoVoto *)malloc(sizeof(CandidatoVoto));
      candidato->nome = malloc(strlen(buff1) + 1);
      strcpy(candidato->nome, buff1);
      candidato->tipoOperazione = buff2[0];

      result_espVoto = esprimi_voto_1(candidato, cl);
      if (result_espVoto == NULL)
      {
        fprintf(stderr, "%s: %s fallisce la rpc in esprimi_voto \n", argv[0], server);
        clnt_perror(cl, server);
      }
      else
      {

        if (*result_espVoto == 0)
        {
          printf("Il candidato non è stato trovato\n");
        }
        if (*result_espVoto == 1)
        {
          printf("modifica del voto effettuata con successo!\n");
        }
      }
    }
    else
    {
      printf("Argomento di ingresso errato!!\n");
    }
    printf("che programma vuoi usare?\nclassifica_giudici (c) | esprimi_voto (e)\n");
  }
  free(buff1);
  free(buff2);
  printf("\nTermino...");
  clnt_destroy(cl);
  exit(0);
}