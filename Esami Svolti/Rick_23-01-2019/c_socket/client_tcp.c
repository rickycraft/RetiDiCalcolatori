/* Client per richiedere l'invio di un file (get, versione 1) */

#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define DIM_BUFF 100

int main(int argc, char *argv[]) {
  int sd, nread, port;
  long int f_size;
  char tmp[DIM_BUFF];
  struct hostent *host;
  struct sockaddr_in servaddr;
#pragma region setup
  /* CONTROLLO ARGOMENTI ---------------------------------- */
  if (argc != 3) {
    printf("client# Error: argc != 3\n");
    exit(1);
  }
  printf("client# Client avviato\n");

  /* PREPARAZIONE INDIRIZZO SERVER ----------------------------- */
  memset((char *)&servaddr, 0, sizeof(struct sockaddr_in));
  servaddr.sin_family = AF_INET;
  host = gethostbyname(argv[1]);
  if (host == NULL) {
    printf("client# %s not found in /etc/hosts\n", argv[1]);
    exit(2);
  }

  nread = 0;
  while (argv[2][nread] != '\0') {
    if ((argv[2][nread] < '0') || (argv[2][nread] > '9')) {
      printf("client# secondo argomento non intero\n");
      exit(2);
    }
    nread++;
  }
  port = atoi(argv[2]);
  if (port < 1024 || port > 65535) {
    printf("client# porta scorretta...");
    exit(2);
  }

  servaddr.sin_addr.s_addr = ((struct in_addr *)(host->h_addr_list[0]))->s_addr;
  servaddr.sin_port = htons(port);

  /* CREAZIONE E CONNESSIONE SOCKET (BIND IMPLICITA) ----------------- */
  sd = socket(AF_INET, SOCK_STREAM, 0);
  if (sd < 0) {
    perror("apertura socket ");
    exit(3);
  }
  printf("client# creata la socket sd=%d\n", sd);

  if (connect(sd, (struct sockaddr *)&servaddr, sizeof(struct sockaddr)) < 0) {
    perror("Errore in connect");
    exit(4);
  }
  printf("client# connect ok\n");

#pragma endregion
  /* CORPO DEL CLIENT: */
  printf("Inserisci targa: ");
  while (gets(tmp)) {
    if (write(sd, tmp, strlen(tmp) + 1) < 0) {
      perror("write");
      break;
    }
    // read all incoming files
    int file;
    char ch;
    while (true) {
      // read file size
      if (read(sd, &f_size, sizeof(long int)) <= 0) break;
      // exit if f_size is not valid
      if (f_size <= 0) break;
      // read file name
      if (read(sd, tmp, sizeof(tmp)) <= 0) break;
      printf("client# file name %s, size %ld\n", tmp, f_size);
      // open file in write mode
      file = open(tmp, O_WRONLY | O_CREAT | O_TRUNC, 0755);
      // read for the set size
      for (int i = 0; i < f_size; i++) {
        if (read(sd, &ch, sizeof(ch)) <= 0) break;
        if (write(file, &ch, sizeof(ch)) < 0) break;
      }
      close(file);
    }

    printf("Inserisci targa: ");
  }

  printf("client# killed\n");
  shutdown(sd, 0);
  shutdown(sd, 1);
  close(sd);
  exit(0);
}
