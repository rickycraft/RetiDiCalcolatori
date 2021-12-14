#include <dirent.h>
#include <fcntl.h>
#include <rpc/rpc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "operazioni.h"
#define GRANDEZZATABELLA 10


typedef struct {

  char *nomeCandidato;
  char *nomeGiudice;
  char categoria;
  char *nomeFile;
  char fase;
  int voto;
}Candidato;

Candidato tabella[GRANDEZZATABELLA];
Giudice listaGiudici[NUMGIUDICI];
static int tabellaGiaFatta = 0;
void creazioneTabella()
{
  Candidato riga;

  //creo la tabella e la riempio di righe vuote
  for (int i = 0; i < GRANDEZZATABELLA; i++)
  {
    tabella[i].nomeCandidato = malloc(2 * sizeof(char));
    tabella[i].nomeGiudice = malloc(2 * sizeof(char));
    tabella[i].nomeFile = malloc(2 * sizeof(char));
    strcpy(tabella[i].nomeCandidato, "L");
    strcpy(tabella[i].nomeGiudice, "L");
    tabella[i].categoria = 'L';
    strcpy(tabella[i].nomeFile, "L");
    tabella[i].fase = 'L';
    tabella[i].voto = -1;
  }
  //ci aggiungo una riga
  free(tabella[0].nomeCandidato);
  free(tabella[0].nomeGiudice);
  free(tabella[0].nomeFile);
  tabella[0].nomeCandidato = malloc(strlen("Zak") + 1);
  tabella[0].nomeFile = malloc(strlen("ZakFile") + 1);
  tabella[0].nomeGiudice = malloc(strlen("SteveGiudice") + 1);
  strcpy(tabella[0].nomeCandidato, "Zak");
  strcpy(tabella[0].nomeGiudice, "SteveGiudice");
  tabella[0].categoria = 'U';
  strcpy(tabella[0].nomeFile, "ZakFile");
  tabella[0].fase = 'B';
  tabella[0].voto = 0;

  free(tabella[1].nomeCandidato);
  free(tabella[1].nomeGiudice);
  free(tabella[1].nomeFile);
  tabella[1].nomeCandidato = malloc(strlen("Steve") + 1);
  tabella[1].nomeFile = malloc(strlen("SteveFile") + 1);
  tabella[1].nomeGiudice = malloc(strlen("ZakGiudice") + 1);
  strcpy(tabella[1].nomeCandidato, "Steve");
  strcpy(tabella[1].nomeGiudice, "ZakGiudice");
  tabella[1].categoria = 'U';
  strcpy(tabella[1].nomeFile, "SteveGiudice");
  tabella[1].fase = 'B';
  tabella[1].voto = 1;

 
  tabellaGiaFatta = 1;
}

/*
restituisce una struttura dati che è la classifica dei giudici in ordine di voti
*/
Output *classifica_giudici_1_svc(void *noPar, struct svc_req *rp)
{
  static Output res;
  int presente, max, count = 0;
  int i, k;
  int presenteInLista;
  int indLista = 0;
  if (tabellaGiaFatta == 0)
  {
    creazioneTabella();
  }

  //inizializzo res
  for (i = 0; i < NUMGIUDICI; i++)
  {
    printf("inizializzo res\n");
    res.giudici_ordinati[i].votiTotali = -1;
    res.giudici_ordinati[i].nome = malloc(strlen("L") + 1);
    strcpy(res.giudici_ordinati[i].nome, "L");
  }
  //inizializzazione di listaGiudici
  for (int i = 0; i < NUMGIUDICI; i++)
  {
    listaGiudici[i].nome = malloc(strlen("L") + 1);
    strcpy(listaGiudici[i].nome, "L");
    listaGiudici[i].votiTotali = 0;
  }

  //riempio listaGiudici con una riga per giudice
  for (int i = 0; i < NUMGIUDICI; i++)
  {
    presenteInLista = 0;

    for (int k = 0; k < NUMGIUDICI; k++)
    {
      if (strcmp(listaGiudici[k].nome, tabella[i].nomeGiudice) == 0)
      {
        listaGiudici[k].votiTotali += tabella[i].voto;
        presenteInLista = 1;
      }
    }
    if (presenteInLista == 0)
    {
      free(listaGiudici[indLista].nome);
      listaGiudici[indLista].nome = malloc(strlen(tabella[i].nomeGiudice) + 1);
      strcpy(listaGiudici[indLista].nome, tabella[i].nomeGiudice);
      listaGiudici[indLista].votiTotali = tabella[i].voto;
      indLista++;
    }
  }

  // ordinamento
  for (i = 0; i < indLista; i++)
  { 
    for (k = 0; k < indLista; k++)
    {

      if (res.giudici_ordinati[i].votiTotali < listaGiudici[k].votiTotali) 
      {

        free(res.giudici_ordinati[i].nome);
        res.giudici_ordinati[i].nome = malloc(strlen(listaGiudici[k].nome) + 1);
        strcpy(res.giudici_ordinati[i].nome, listaGiudici[k].nome);
        res.giudici_ordinati[i].votiTotali = listaGiudici[k].votiTotali;
        count = k;
      }
    }
    listaGiudici[count].votiTotali = 0; //per non riconsiderarlo
  }
  for (i = 0; i < NUMGIUDICI; i++)
  {
    printf("%d\n", res.giudici_ordinati[i].votiTotali);
    printf("%s\n", res.giudici_ordinati[i].nome);
  }
  return &res;
}

/*
parametro d’ingresso il nome del candidato e se si vuole aggiungere ridurre il voto
*/

int *esprimi_voto_1_svc(CandidatoVoto *candidato, struct svc_req *rp)
{
  //return 1 se è andato tutto bene, 0 se non ha trovato il candidato
  static int n = 0;
  if (tabellaGiaFatta == 0)
  {
    creazioneTabella();
  }

  printf("Ricevuto input candidato %s operazione %c \n", candidato->nome, candidato->tipoOperazione);

  for (int i = 0; i < GRANDEZZATABELLA; i++)
  {
    //supponendo che il nome del candidato sia univoco
    if (strcmp(tabella[i].nomeCandidato, candidato->nome) == 0)
    {
      //abbiamo trovato il candidato giusto
      if ((candidato->tipoOperazione) == '+')
      {
        tabella[i].voto++;
        n = 1;
      }
      if ((candidato->tipoOperazione) == '-')
      {
        tabella[i].voto--;
        n = 1;
      }
    }
  }
  return &n;
}