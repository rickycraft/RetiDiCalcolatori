#include <dirent.h>
#include <fcntl.h>
#include <rpc/rpc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "operazioni.h"
#define GRANDEZZA_TABELLA

Candidato tabella[GRANDEZZA_TABELLA];
//Giudice listaGiudici[NUMGIUDICI];
static int tabellaGiaFatta = 0;
static int n = 0;

void creazioneTabella()
{

  //creo la tabella e la riempio di righe
  for (int i = 0; i < GRANDEZZA_TABELLA; i++)
  {
    tabella[i].durata = 0;
    strcpy(tabella[i].nome, "L");
    strcpy(tabella[i].nomeFile, "L");
    for (int j = 0; j < NUM_PAROLE; j++)
    {
      strcpy(tabella[i].p[j].parola, "L");
    }
  }

  //ci aggiungo una
  strcpy(tabella[0].nome, "01012000_TalentScout_Talent");
  strcpy(tabella[0].nomeFile, "nomeFile1");
  strcpy(tabella[0].p[0].parola, "parola1");
  //tabella[0].p[1].parola = malloc(strlen("parola2")+1);
  //strcpy(tabella[0].p[1].parola, "parola2");

  strcpy(tabella[1].nome, "31122021_TalentScout_Talent");
  strcpy(tabella[1].nomeFile, "nomeFile2");
  strcpy(tabella[1].p[0].parola, "parola1Riga2");

  tabellaGiaFatta = 1;
}

/*
restituisce una struttura dati che è la classifica dei giudici in ordine di 
*/
Tabella *ricerca_provini_1_svc(Periodo *periodo, struct svc_req *rp)
{
  static Tabella res;
  int presente, max, count = 0;
  int i, k;
  int presenteInLista;
  int indLista = 0;
  if (tabellaGiaFatta == 0)
  {
    creazioneTabella();
  }

  for (i = 0; i < 8; i++)
  {
    res.candidato[i].durata = 0;
    strcpy(res.candidato[i].nome, "L");
    strcpy(res.candidato[i].p->parola[0], "L");
    strcpy(res.candidato[i].p->parola[1], "L");
  }

  char data[6];
  char **split;
  for (i = 0; i < GRANDEZZA_TABELLA; i++)
  {
    split = strtok(tabella[i].nome, '_');
    char *data = split[0]; //
    char *anno;
    char *mese;
    char *giorno;
    char **periodoIniziale = strtok(periodo.dataIniziale, '/');
    char **periodoFinale = strtok(periodo.dataFinale, '/');
    strcat(giorno, data[0]);
    strcat(giorno, data[1]);

    strcat(mese, data[2]);
    strcat(mese, data[3]);

    strcat(anno, data[4]);
    strcat(anno, data[5]);
    strcat(anno, data[6]);
    strcat(anno, data[7]);

    if (strcmp(anno, periodoIniziale[2]) >= 0)
    {
      //stesso anno di
      if (strcmp(anno, periodoFinale[2]) <= 0)
      {
        if (strcmp(mese, periodoIniziale[1]) >= 0)
        {
          if (strcmp(mese, periodoFinale[1]) <= 0)
          {
            if (strcmp(mese, periodoIniziale[1]) == 0)
            {
              //check del
              if (strcmp(giorno, periodoIniziale[0]) >= 0)
              {
                //giorno
                //
                res.candidato[count].durata = tabella[i].durata;
                strcpy(res.candidato[count].nome, tabella[i].nome);
                res.candidato[count].p = tabella[i].p;
                count++
              }
            }

            {
              //se è di un mese successivo non contorllo il giorno che non mi
              if (strcmp(mese, periodoFinale[1]) == 0)
              {
                //mi assicuro che il giorno sia prima della fine del periodo o che sia
                if (strcmp(giorno, periodoFinale[1]) <= 0)
                {
                  //

                  res.candidato[count].durata = tabella[i].durata;
                  strcpy(res.candidato[count].nome, tabella[i].nome);
                  res.candidato[count].p = tabella[i].p;
                  count++
                }
              }

              {
                //è ok così perchè minore del
                //

                res.candidato[count].durata = tabella[i].durata;
                strcpy(res.candidato[count].nome, tabella[i].nome);
                res.candidato[count].p = tabella[i].p;
                count++
              }
            }
          }
        }
      }

      return &res;
    }

    /*
parametro d’ingresso il nome del candidato e se si vuole aggiungere ridurre il 
*/

    int *aggiungi_parola_1_svc(CandidatoParola * candidato, struct svc_req * rp)
    {

      return &n;
    }