#include <stdio.h>

#include "esame_140711.h"

#define BUFF_DIM 100

int main(int argc, char *argv[])
{
  CLIENT *cl;
  char *server, *buff1, *buff2;
  int soglia;
  void *in;
  Tabella *result;
  CandidatoParola *candidato;
  int *result_aggParola;
  buff1 = malloc(BUFF_DIM * sizeof(char));
  
  char c[256];
  if (argc != 2)
  {
    printf("usage: %s server_host\n", argv[0]);
    exit(1);
  }
  server = argv[1];
  cl = clnt_create(server, PROVINIPROG, PROVINIVERS, "udp");
  if (cl == NULL)
  {
    clnt_pcreateerror(server);
    exit(1);
  }
  //printf("[DEBUG] creato trasporto\n");

  printf("che programma vuoi usare?\nricerca_provini (r) | aggiungi_parola (a)\n");
  while (gets(c))
  {
    if (c[0] == 'r')
    { //classifica Giudici
      Periodo *p;
      p=(Periodo *)malloc(sizeof(Periodo));
      
      printf("Dammi le data di inizio periodo nel seguente formato gg/mm/aaaa\n");
      gets(buff1);
      while(strlen(buff1)!=10){
        printf("scrivi correttamente la data:\n");
        gets(buff1);
      }
      strcpy(p->dataIniziale,buff1);
      //strcat(p->dataIniziale,"\0");

      printf("Dammi le data di fine periodo nel seguente formato gg/mm/aaaa\n");
      gets(buff1);
      while(strlen(buff1)!=10){
        printf("scrivi correttamente la data:\n");
        gets(buff1);
      }
      strcpy(p->dataFinale,buff1);
      //strcat(p->dataFinale,'\0');

      result = ricerca_provini_1(p, cl);
      
      if (result == (Tabella *)NULL)
      {
        fprintf(stderr, "%s: %s fallisce la rpc in classifica_giudici \n", argv[0], server);
        clnt_perror(cl, server);
      }

      printf("Concorrenti nel periodo chiesto \n");
      for (int i = 0; i < 8; i++)
      {
        if (strcmp(result->candidato[i].nome, "L") != 0)
        {
          printf("nome:%s \tdurata:%i", (result->candidato[i].nome),result->candidato[i].durata);
          for(int j=0;j<NUM_PAROLE;j++){
            printf("parola%i : %s \t",j,result->candidato[i].p[j].parola);
          }
          printf("\n");
        }
      }
      free(p);
    }
    else if (c[0] == 'a')
    {
      candidato=(CandidatoParola *)malloc(sizeof(CandidatoParola));
      printf("Digita il nome del concorrente(ggmmaaa_talentScout_talent): \n");
      gets(buff1);
      strcpy(candidato->nome,buff1);
      printf("Quale parola vuoi aggiungere? \n");
      gets(buff1);
      strcpy(candidato->p.parola,buff1);
      //candidato=(CandidatoVoto *)malloc(sizeof(CandidatoVoto));
      //candidato->nome = malloc(strlen(buff1) + 1);
      //strcpy(candidato->nome, buff1);
      //candidato->tipoOperazione = buff2[0];

      result_aggParola = aggiungi_parola_1(candidato, cl);
      if (result_aggParola == NULL)
      {
        fprintf(stderr, "%s: %s fallisce la rpc in esprimi_voto \n", argv[0], server);
        clnt_perror(cl, server);
      }
      else
      {

        if (*result_aggParola == 0)
        {
          printf("Il candidato non è stato trovato o non ha più spazio\n");
        }
        if (*result_aggParola == 1)
        {
          printf("aggiunta della parola effettuata con successo!\n");
        }


      }
      free(candidato);
    }
    else
    {
      printf("Argomento di ingresso errato!!\n");
    }
  printf("che programma vuoi usare?\nricerca_provini (r) | aggiungi_parola (a)\n");
  }
  free(buff1);
  printf("\nTermino...");
  clnt_destroy(cl);
  exit(0);
}