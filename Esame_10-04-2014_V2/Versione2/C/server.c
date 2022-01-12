#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define DIM_BUFF 100
#define LENGTH_FILE_NAME 20
#define max(a, b) ((a) > (b) ? (a) : (b))
#define MAX_LENGTH_STRING 256
#define MAX_DATI 5
#define DIM_RESULT 512

typedef struct
{
  char id[MAX_LENGTH_STRING];
  char tipo[MAX_LENGTH_STRING];
  int bigliettiDisponibili;
  int prezzo;
  char luogo[MAX_LENGTH_STRING];
  char data[11];
} Evento;

/********************************************************/
void gestore(int signo)
{
  int stato;
  printf("server# esecuzione gestore di SIGCHLD\n");
  wait(&stato);
}
/********************************************************/

int main(int argc, char **argv)
{
  int listenfd, connfd, udpfd, nready, maxfdp1;
  const int on = 1;
  fd_set rset;
  int len, nwrite, port;
  char tmp[DIM_BUFF];
  char result[DIM_RESULT];
  static Evento dati[MAX_DATI];
  struct sockaddr_in cliaddr, servaddr;

#pragma region setup
  /* CONTROLLO ARGOMENTI ---------------------------------- */
  if (argc != 2)
  {
    printf("server# Error: argc != 2 %s\n", argv[0]);
    exit(1);
  }

  port = atoi(argv[1]);
  if (port < 1024 || port > 65535)
  {
    printf("server# Porta scorretta...");
    exit(2);
  }

  /*Inizializzazione struttura dati*/
  for (int i = 0; i < MAX_DATI; i++)
  {
    strcpy(dati[i].id, "L");
    strcpy(dati[i].tipo, "L");
    strcpy(dati[i].data, "L");
    strcpy(dati[i].luogo, "L");
    dati[i].prezzo = 0;
    dati[i].bigliettiDisponibili = 0;
  }

  strcpy(dati[0].id, "Evento1");
  strcpy(dati[0].tipo, "Concerto");
  strcpy(dati[0].data, "12/12/1212");
  strcpy(dati[0].luogo, "Roma");
  dati[0].prezzo = 15;
  dati[0].bigliettiDisponibili = 15;

  strcpy(dati[1].id, "Evento2");
  strcpy(dati[1].tipo, "Formula1");
  strcpy(dati[1].data, "12/12/1212");
  strcpy(dati[1].luogo, "Torriana");
  dati[1].prezzo = 175;
  dati[1].bigliettiDisponibili = 15312;

  strcpy(dati[2].id, "Evento3");
  strcpy(dati[2].tipo, "Calcio");
  strcpy(dati[2].data, "12/12/1212");
  strcpy(dati[2].luogo, "Bologna");
  dati[2].prezzo = 100;
  dati[2].bigliettiDisponibili = 100;

  /* INIZIALIZZAZIONE INDIRIZZO SERVER E BIND ---------------------------- */
  memset((char *)&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(port);
  printf("server# avviato\n");

  /* CREAZIONE SOCKET TCP ------------------------------------------------ */
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  if (listenfd < 0)
  {
    perror("apertura socket TCP ");
    exit(1);
  }
  printf("server# creata socket TCP, fd=%d\n", listenfd);

  if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
  {
    perror("set opzioni socket TCP");
    exit(2);
  }
  printf("server# set TCP socket options\n");

  if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
  {
    perror("bind socket TCP");
    exit(3);
  }
  printf("server# bind socket TCP ok\n");

  if (listen(listenfd, 5) < 0)
  {
    perror("listen");
    exit(4);
  }
  printf("server# listen ok\n");

  /* CREAZIONE SOCKET UDP ------------------------------------------------ */
  udpfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (udpfd < 0)
  {
    perror("apertura socket UDP");
    exit(5);
  }
  printf("server# creata socket UDP, fd=%d\n", udpfd);

  if (setsockopt(udpfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
  {
    perror("set opzioni socket UDP");
    exit(6);
  }
  printf("server# set opzioni socket UDP ok\n");

  if (bind(udpfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
  {
    perror("bind socket UDP");
    exit(7);
  }
  printf("server# bind socket UDP ok\n");

  /* AGGANCIO GESTORE PER EVITARE FIGLI ZOMBIE --------------------------------
   */
  signal(SIGCHLD, gestore);

  /* PULIZIA E SETTAGGIO MASCHERA DEI FILE DESCRIPTOR -------------------------
   */
  FD_ZERO(&rset);
  maxfdp1 = max(listenfd, udpfd) + 1;

/* CICLO DI RICEZIONE EVENTI DALLA SELECT -----------------------------------
 */
#pragma endregion
  for (;;)
  {
    FD_SET(listenfd, &rset);
    FD_SET(udpfd, &rset);

    if ((nready = select(maxfdp1, &rset, NULL, NULL, NULL)) < 0)
    {
      if (errno == EINTR)
        continue;
      else
      {
        perror("select");
        exit(8);
      }
    }

    /* GESTIONE TCP
     * ------------------------------------- */
    if (FD_ISSET(listenfd, &rset))
    {
      len = sizeof(struct sockaddr_in);
      // ACCETTA CONNESSIONE TCP
      if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len)) < 0)
      {
        if (errno == EINTR)
          continue;
        else
        {
          perror("accept");
          exit(9);
        }
      }

      // GESTIONE TCP NEL FIGLIO
      if (fork() == 0)
      {
        close(listenfd);
        printf("server# accettata connessione, pid=%i\n", getpid());
        while (read(connfd, tmp, sizeof(tmp)) > 0)
        {
          char *copiadiTemp=malloc(strlen(tmp)+1);
          strcpy(copiadiTemp, tmp);
          memset(result, 0, sizeof(result));
          char *tipo = strtok(copiadiTemp, " ");
          char *luogo = strtok(NULL, " ");
          for (int i = 0; i < MAX_DATI; i++)
          {
            if (strcmp(dati[i].id, "L") != 0)
            {
              if (strcmp(dati[i].luogo, luogo) == 0 && strcmp(dati[i].tipo, tipo) == 0)
              {
                printf("trovata occorrenza");
              char *num;
              sprintf(num,"%d",dati[i].prezzo);
                strcat(result, dati[i].id);
                strcat(result, " ");
                strcat(result, dati[i].tipo);
                strcat(result, " ");
                strcat(result, dati[i].luogo);
                strcat(result, " ");
                strcat(result, dati[i].data);
                strcat(result, " ");
                strcat(result, num);
                sprintf(num,"%d",dati[i].bigliettiDisponibili);
                strcat(result, " ");
                strcat(result, num);
                strcat(result, "\n");
              }
            }
          }

          printf("Contenuto che mando:\n%s",result);
          if ((nwrite = write(connfd, result, strlen(result) + 1)) < 0)
          {
            perror("write");
            break;
          }
        }

        printf("server# chiusa connessione, pid=%i\n", getpid());
        shutdown(connfd, 0);
        shutdown(connfd, 1);
        close(connfd);
        exit(0);
      }
    }

    /* GESTIONE UDP
     * ------------------------------------------ */
    if (FD_ISSET(udpfd, &rset))
    {
      len = sizeof(struct sockaddr_in);
      while (recvfrom(udpfd, tmp, sizeof(tmp), 0, (struct sockaddr *)&cliaddr, &len) >= 0)
      {

        int prezzo = atoi(tmp);

        for (int i = 0; i < MAX_DATI; i++)
        {
          if (strcmp(dati[i].id, "L") != 0)
          {
            if (dati[i].prezzo <= prezzo)
            {
              printf("trovata occorrenza\n");
              char *num=malloc(sizeof(dati[i].prezzo));
              sprintf(num,"%d",dati[i].prezzo);

              strcat(result, dati[i].id);
              strcat(result, " ");
              strcat(result, dati[i].tipo);
              strcat(result, " ");
              strcat(result, dati[i].luogo);
              strcat(result, " ");
              strcat(result, dati[i].data);
              strcat(result, " ");
              strcat(result, num);
              strcat(result, " ");
              free(num);
              num=malloc(sizeof(dati[i].bigliettiDisponibili));
              sprintf(num,"%d",dati[i].bigliettiDisponibili);
              strcat(result,num);
              strcat(result, "\n");
            }
          }
        }

        printf("sto per mandare al client il seguente contenuto:\n%s",result);
        if (sendto(udpfd, result, strlen(result) + 1, 0, (struct sockaddr *)&cliaddr,
                   len) < 0)
        {
          perror("sendto");
          continue;
        }
      }
    }
  } /* ciclo for della select */
  printf("server# killed");
  exit(0);
}
