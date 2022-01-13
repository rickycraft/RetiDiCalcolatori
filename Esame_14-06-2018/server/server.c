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
#define LENGTH_DIR 128
#define max(a, b) ((a) > (b) ? (a) : (b))
#define MAX_LENGTH_STRING 10
#define LENGTH_PATENTE 5
#define MAX_DATI 5
#define DIM_RESULT 512

typedef struct
{
  char targa[MAX_LENGTH_STRING];
  char tipo[MAX_LENGTH_STRING];
  char patente[LENGTH_PATENTE];
  char dir_img[LENGTH_DIR];
} Prenotazione;

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
  static Prenotazione dati[MAX_DATI];
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
        DIR *d;
        struct dirent *dir;
        printf("server# accettata connessione, pid=%i\n", getpid());
        //char precedenteDir[256];
        printf("PRE READ\n");
        int n;
        while ((n = read(connfd, tmp, sizeof(tmp))) > 0)
        {
          printf("caratteri:%d\n", n);
          printf("ricevuto:%s\n", tmp);

          for (int i = 0; i < MAX_DATI; i++)
          {
            printf("DENTRO IL FOR\n");
            printf("Targa\tTipo\tPatente\tDIR\n");
            for (int i = 0; i < MAX_DATI; i++)
            {
              printf("Targa: %s ricevuto: %s\n", dati[i].targa,tmp);
            }
            int c=strcmp(dati[i].targa, tmp);
            printf("strcmp:%d\n",c);
            if (c == 0)
            {
              printf("trovata occorrenza");
              //getcwd(precedenteDir, sizeof(precedenteDir));
              char *directory = malloc(strlen(dati[i].dir_img) + strlen("./") + 1);
              d = opendir(directory);
              int fd;
              long lunghezzaFile;
              int fileRimanente;

              while ((dir = readdir(d)) != NULL)
              {
                if (dir->d_type == DT_REG)
                {

                  fd = open(dir->d_name, O_RDONLY);
                  write(connfd, "|", 1);
                  write(connfd, dir->d_name, strlen(dir->d_name));
                  write(connfd, "*", 1);
                  fseek(fd, 0, SEEK_END);
                  lunghezzaFile = ftell(fd);
                  fseek(fd, 0, SEEK_SET);
                  write(connfd, lunghezzaFile, sizeof(lunghezzaFile));
                  write(connfd, "|", 1);
                  fileRimanente = lunghezzaFile;
                  for (i = 0; fileRimanente > 0; i++)
                  {

                    if (fileRimanente < 64)
                    {
                      read(fd, result, fileRimanente);
                      write(connfd, result, fileRimanente);
                      fileRimanente = 0;
                    }
                    else
                    {

                      read(fd, result, 64);
                      write(connfd, result, 64);
                      fileRimanente -= 64;
                    }
                  }
                }
              }
            }
          }

         /* printf("Contenuto che mando:\n%s", result);
          if ((nwrite = write(connfd, result, strlen(result) + 1)) < 0)
          {
            perror("write");
            break;
          }*/
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
      //RICEVO PATENTE e TARGA. RESTITUISCO OK SE RIUSCITO
      len = sizeof(struct sockaddr_in);
      while (recvfrom(udpfd, tmp, sizeof(tmp), 0, (struct sockaddr *)&cliaddr, &len) >= 0)
      {

        char *patente = strtok(tmp, " ");
        printf("patente: %s\n", patente);
        char *targa = strtok(NULL, " ");
        printf("targa: %s\n", targa);
        strcpy(result, "ERROR");
        for (int i = 0; i < MAX_DATI; i++)
        {
          if (strcmp(dati[i].targa, targa) == 0)
          {
            strcpy(dati[i].patente, patente);
            strcpy(result, "OK");
          }
        }

        printf("sto per mandare al client il seguente contenuto:\n%s", result);
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
