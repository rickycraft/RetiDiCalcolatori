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

/********************************************************/
void gestore(int signo) {
  int stato;
  printf("server# esecuzione gestore di SIGCHLD\n");
  wait(&stato);
}
/********************************************************/

int main(int argc, char **argv) {
  int listenfd, connfd, udpfd, nready, maxfdp1;
  const int on = 1;
  fd_set rset;
  int len, nwrite, port;
  char tmp[DIM_BUFF];
  struct sockaddr_in cliaddr, servaddr;

#pragma region setup
  /* CONTROLLO ARGOMENTI ---------------------------------- */
  if (argc != 2) {
    printf("server# Error: argc != 2\n", argv[0]);
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
        printf("server# accettata connessione, pid=%i\n", getpid());
        while (read(connfd, tmp, sizeof(tmp)) > 0) {
          if ((nwrite = write(connfd, tmp, strlen(tmp) + 1)) < 0) {
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
    if (FD_ISSET(udpfd, &rset)) {
      len = sizeof(struct sockaddr_in);
      while (recvfrom(udpfd, tmp, sizeof(tmp), 0, (struct sockaddr *)&cliaddr,
                      &len) >= 0) {
        if (sendto(udpfd, tmp, strlen(tmp) + 1, 0, (struct sockaddr *)&cliaddr,
                   len) < 0) {
          perror("sendto");
          continue;
        }
      }
    }
  } /* ciclo for della select */
  printf("server# killed");
  exit(0);
}
