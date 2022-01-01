#include <dirent.h>
#include <fcntl.h>
#include <rpc/rpc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "esame_140711.h"
#define GRANDEZZA_TABELLA 5

Candidato tabella[GRANDEZZA_TABELLA];
//Giudice listaGiudici[NUMGIUDICI];
static int tabellaGiaFatta = 0;
static int n = 0;

void creazioneTabella()
{
  printf("inizio tabella\n");
  //creo la tabella e la riempio di righe
  for (int i = 0; i < GRANDEZZA_TABELLA; i++)
  {
    tabella[i].durata = 0;
    strcpy(tabella[i].nome, "L_L_L");
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
  printf("Tabella Creata\n");
  tabellaGiaFatta = 1;
}

Tabella *ricerca_provini_1_svc(Periodo *periodo, struct svc_req *rp)
{
static Tabella res;
printf("indirizzo res:%p\n", (void *)&res); 

  int presente, max, count = 0;
  int i, k;
  int presenteInLista;
  char *data;

  if (tabellaGiaFatta == 0)
  {
    creazioneTabella();
  }
  // printf("init ricerca provini");
  for (i = 0; i < 8; i++)
  {
    res.candidato[i].durata = 0;
    strcpy(res.candidato[i].nome, "L");
    strcpy(res.candidato[i].p[0].parola, "L");
    strcpy(res.candidato[i].p[1].parola, "L");
  }
  // printf("res inizializzato\n");

  for (int j = 0; j < NUM_PAROLE; j++)
  {
    strcpy(res.candidato[count].p[j].parola, tabella[i].p[j].parola);
  }
  // printf("riga72\n");
  // printf("Pre strtok\n");

  char *giornoIniziale = strtok(periodo->dataIniziale, "/");
  // printf("giornoIniziale:%s\n", giornoIniziale);
  char *meseIniziale = strtok(NULL, "/");
  //  printf("meseIniziale:%s\n", meseIniziale);
  char *annoIniziale = strtok(NULL, "/");
  // printf("annoIniziale:%s\n", annoIniziale);

  char *giornoFinale = strtok(periodo->dataFinale, "/");
  // printf("giornoFinale:%s\n", giornoFinale);
  char *meseFinale = strtok(NULL, "/");
  // printf("meseFinale:%s\n", meseFinale);
  char *annoFinale = strtok(NULL, "/");
  // printf("annoFinale:%s\n", annoFinale);
  //  printf("POST strtok\n");
  for (i = 0; i < GRANDEZZA_TABELLA; i++)
  {
    presenteInLista = 0;
    //printf("nomePretok: %s", tabella[i].nome);
    char *nomeTemp;
    strcpy(nomeTemp, tabella[i].nome);
    data = strtok(nomeTemp, "_"); //a quanto pare, dopo strtok in tabella[i].nome viene salvato la prima stringa splittata

    //printf("nomePostTok: %s", tabella[i].nome);
    char anno[5];   //del concorrente
    char mese[3];   //del concorrente
    char giorno[3]; //del concorrente

    if (strcmp(data, "L") != 0)
    {
      // printf("data:%s\n", data);
      /*for (int i = 0; i < 8; i++)
      {
        printf("%c,", data[i]);
      }
      printf("\n");*/

      strncpy(giorno, data, 2);
      giorno[2] = '\0';

      strncpy(mese, data + 2, 2);
      mese[2] = '\0';

      strncpy(anno, data + 4, 4);
      anno[4] = '\0';

      //printf("Anno del concorrente: %s\n", anno);

      //printf("Mese del concorrente: %s\n", mese);

      //printf("Giorno del concorrente: %s\n", giorno);
      if (strcmp(anno, annoIniziale) >= 0)
      {
        //stesso anno di
        if (strcmp(anno, annoFinale) <= 0)
        {
          if (strcmp(mese, meseIniziale) >= 0)
          {
            //if (strcmp(mese, meseFinale) <= 0)
            //{
              if (strcmp(mese, meseIniziale) == 0)
              {
                //check del
                if (strcmp(giorno, giornoIniziale) >= 0)
                {
                  //giorno
                  if (presenteInLista == 0)
                  {
                    //printf("preCopia\n durata:%i, nome: %s, parola1:%s, parola2:%s\n", tabella[i].durata, tabella[i].nome, tabella[i].p[0].parola, tabella[i].p[1].parola);
                    res.candidato[count].durata = tabella[i].durata;
                    strcpy(res.candidato[count].nome, tabella[i].nome);
                    for (int j = 0; j < NUM_PAROLE; j++)
                    {
                      strcpy(res.candidato[count].p[j].parola, tabella[i].p[j].parola);
                    }
                    count++;
                    presenteInLista=1;
                  }
                }
              }

              {
                //se è di un mese successivo non contorllo il giorno che non mi
                if (strcmp(mese, meseFinale) == 0)
                {
                  //mi assicuro che il giorno sia prima della fine del periodo o che sia
                  if (strcmp(giorno, giornoFinale) <= 0)
                  {
                    if (presenteInLista == 0)
                    {
                      //printf("preCopia\n durata:%i, nome: %s, parola1:%s, parola2:%s\n", tabella[i].durata, tabella[i].nome, tabella[i].p[0].parola, tabella[i].p[1].parola);
                      res.candidato[count].durata = tabella[i].durata;
                      strcpy(res.candidato[count].nome, tabella[i].nome);
                      for (int j = 0; j < NUM_PAROLE; j++)
                      {
                        strcpy(res.candidato[count].p[j].parola, tabella[i].p[j].parola);
                      }
                      count++;
                      presenteInLista=1;
                    }
                  }
                }

                {
                  //è ok così perchè minore del
                  //

                  if (presenteInLista == 0)
                  {
                    //printf("preCopia\n durata:%i, nome: %s, parola1:%s, parola2:%s\n", tabella[i].durata, tabella[i].nome, tabella[i].p[0].parola, tabella[i].p[1].parola);
                    res.candidato[count].durata = tabella[i].durata;
                    strcpy(res.candidato[count].nome, tabella[i].nome);
                    for (int j = 0; j < NUM_PAROLE; j++)
                    {
                      strcpy(res.candidato[count].p[j].parola, tabella[i].p[j].parola);
                    }
                    count++;
                    presenteInLista=1;
                  }
                }
              }
            //}
          }
        }
      }
    }
  }

  /*printf("stampa res\n");
  for (i = 0; i < 8; i++)
  {
    if (strcmp(res.candidato[i].nome, "L") != 0)
    {
      printf("nome: %s\t", res.candidato[i].nome);
      for (int j = 0; j < 2; j++)
      {
        printf("parola%i:%s\t", j, res.candidato[i].p[j].parola);
      }
      printf("durata:%i\n", res.candidato[i].durata);
    }
  }*/
  //printf("indirizzo res:%p\n", (void *)&res); 

  return &res;
}

int *aggiungi_parola_1_svc(CandidatoParola *input, struct svc_req *rp)
{
  n = 0;
  int usertrovato=0;
  for (int i = 0; i < GRANDEZZA_TABELLA; i++)
  {
    if (strcmp(tabella[i].nome, input->nome) == 0)
    {
      usertrovato=1;
      for (int j = 0; j < NUM_PAROLE; j++)
      {
        if (strcmp(tabella[i].p[j].parola, "L") == 0)
        {
          strcpy(tabella[i].p[j].parola, input->p.parola);
          n = 1;
          break;
        }
      }
    }
  }

  return &n;
}
