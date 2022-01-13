#undef __USE_MISC
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
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define DIM_BUFF 100
#define LENGTH_FILE_NAME 20
#define max(a, b) ((a) > (b) ? (a) : (b))

#define N_TARGA 7
#define N_PATENTE 5
#define N_VEICOLO 6
const int N = 10;
typedef struct {
  char *targa;
  char *patente;
  char *tipo_veicolo;
  char *folder_immagini;
} Prenotazione;

/********************************************************/
void gestore(int signo) {
  int stato;
  printf("\nserver# esecuzione gestore di SIGCHLD\n");
  wait(&stato);
}

long int stat_filesize(char *filename) {
  struct stat statbuf;
  if (stat(filename, &statbuf) == -1) {
    printf("failed to stat %s\n", filename);
    return -1;
  }
  return statbuf.st_size;
}
/********************************************************/

int main(int argc, char **argv) {
  int listenfd, connfd, udpfd, nready, maxfdp1, fd_file;
  const int on = 1;
  fd_set rset;
  unsigned int len, port, res;
  long int f_size;
  char buff[N_TARGA + N_PATENTE + 1], fbuff[100];
  char targa[N_TARGA + 1], patente[N_PATENTE + 1];
  struct sockaddr_in cliaddr, servaddr;
  Prenotazione prenotazioni[N];
  struct dirent *dir_file;
  int nread;

  // setup struttura dati;
  for (int i = 0; i < N; i++) {
    prenotazioni[i].targa = (char *)malloc(sizeof(char) * (N_TARGA + 1));
    prenotazioni[i].patente = (char *)malloc(sizeof(char) * (N_PATENTE + 1));
    prenotazioni[i].tipo_veicolo =
        (char *)malloc(sizeof(char) * (N_VEICOLO + 1));
    prenotazioni[i].folder_immagini =
        (char *)malloc(sizeof(char) * (N_TARGA + 6));
    strcpy(prenotazioni[i].targa, "L");
    strcpy(prenotazioni[i].patente, "0");
    strcpy(prenotazioni[i].tipo_veicolo, "L");
    strcpy(prenotazioni[i].folder_immagini, "L");
  }
  strcpy(prenotazioni[0].targa, "AA000AA");
  strcpy(prenotazioni[0].patente, "12125");
  strcpy(prenotazioni[0].folder_immagini, "AA000AA_img");

#pragma region setup
  /* CONTROLLO ARGOMENTI ---------------------------------- */
  if (argc != 2) {
    printf("server# Error: argc != 2\n");
    exit(1);
  }

  port = atoi(argv[1]);
  if (port < 1024 || port > 65535) {
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
  if (listenfd < 0) {
    perror("apertura socket TCP ");
    exit(1);
  }
  printf("server# creata socket TCP, fd=%d\n", listenfd);

  if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
    perror("set opzioni socket TCP");
    exit(2);
  }
  printf("server# set TCP socket options\n");

  if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    perror("bind socket TCP");
    exit(3);
  }
  printf("server# bind socket TCP ok\n");

  if (listen(listenfd, 5) < 0) {
    perror("listen");
    exit(4);
  }
  printf("server# listen ok\n");

  /* CREAZIONE SOCKET UDP ------------------------------------------------ */
  udpfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (udpfd < 0) {
    perror("apertura socket UDP");
    exit(5);
  }
  printf("server# creata socket UDP, fd=%d\n", udpfd);

  if (setsockopt(udpfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
    perror("set opzioni socket UDP");
    exit(6);
  }
  printf("server# set opzioni socket UDP ok\n");

  if (bind(udpfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
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
  for (;;) {
    FD_SET(listenfd, &rset);
    FD_SET(udpfd, &rset);

    if ((nready = select(maxfdp1, &rset, NULL, NULL, NULL)) < 0) {
      if (errno == EINTR)
        continue;
      else {
        perror("select");
        exit(8);
      }
    }

    /* GESTIONE TCP
     * ------------------------------------- */
    if (FD_ISSET(listenfd, &rset)) {
      len = sizeof(struct sockaddr_in);
      // ACCETTA CONNESSIONE TCP
      if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len)) < 0) {
        if (errno == EINTR)
          continue;
        else {
          perror("accept");
          exit(9);
        }
      }

      // GESTIONE TCP NEL FIGLIO
      if (fork() == 0) {
        close(listenfd);
        printf("\nserver# accettata connessione, pid=%i\n", getpid());
        while (read(connfd, buff, sizeof(buff)) > 0) {
          // TODO lookup della directory
          // apro directory
          DIR *pDir = opendir(buff);
          if (pDir == NULL) {
            f_size = -1;
            write(connfd, &f_size, sizeof(f_size));
            printf("server# failed to open %s\n", buff);
            continue;
          };
          // Process each entry.
          while ((dir_file = readdir(pDir)) != NULL) {
            // DT_REG = 8
            if (dir_file->d_type == 8) {
              // create filename
              strcpy(fbuff, buff);
              strcat(fbuff, dir_file->d_name);
              fd_file = open(fbuff, O_RDONLY);
              if (fd_file > 0) {
                // scrivo la dimensione
                f_size = stat_filesize(fbuff);
                write(connfd, &f_size, sizeof(long int));
                // scrivo il nome del file
                write(connfd, dir_file->d_name, sizeof(fbuff));
                // scrivo il contenuto del file
                while ((nread = read(fd_file, fbuff, sizeof(fbuff))) > 0) {
                  write(connfd, fbuff, nread);
                }
                close(fd_file);
              }
            }
          }
          // signal to the client we are done
          f_size = -1;
          write(connfd, &f_size, sizeof(f_size));
          closedir(pDir);
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
    if (FD_ISSET(udpfd, &rset)) {
      len = sizeof(struct sockaddr_in);
      res = -1;
      if (recvfrom(udpfd, buff, sizeof(buff), 0, (struct sockaddr *)&cliaddr,
                   &len) == sizeof(buff)) {
        strncpy(targa, &buff[0], N_TARGA);
        targa[N_TARGA] = '\0';
        strcpy(patente, &buff[N_TARGA]);
        printf("\nserver# Ricevuto: buffer: %s, targa: %s, patente %s\n", buff,
               targa, patente);
      } else {
        perror("server# Udp cannot read bytes");
        sendto(udpfd, &res, sizeof(res), 0, (struct sockaddr *)&cliaddr, len);
        continue;
      }
      for (int i = 0; i < N; i++) {
        if (strcmp(prenotazioni[i].targa, targa) == 0) {
          printf("server# Trovata targa %d\n", i);
          strcpy(prenotazioni[i].patente, patente);
          res = 0;
          break;
        }
      }
      if (sendto(udpfd, &res, sizeof(res), 0, (struct sockaddr *)&cliaddr,
                 len) < 0) {
        perror("server# Error sending udp response");
      }
    }
    // break;
  } /* ciclo for della select */
  printf("server# killed");
  exit(0);
}
