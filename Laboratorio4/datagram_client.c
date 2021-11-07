#include <dirent.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define LENGTH_FILE_NAME 20

int main(int argc, char **argv) {
  struct hostent *host;
  struct sockaddr_in clientaddr, servaddr;
  int sd, port, result, len;
  char nome_file[LENGTH_FILE_NAME], parola[LENGTH_FILE_NAME];
  char *separator = "|";

#pragma region setup
  /* CONTROLLO ARGOMENTI ---------------------------------- */
  if (argc != 3) {
    printf("Error:%s serverAddress serverPort\n", argv[0]);
    exit(1);
  }

  /* INIZIALIZZAZIONE INDIRIZZO SERVER--------------------- */
  memset((char *)&servaddr, 0, sizeof(struct sockaddr_in));
  servaddr.sin_family = AF_INET;
  host = gethostbyname(argv[1]);
  if (host == NULL) {
    printf("%s not found in /etc/hosts\n", argv[1]);
    exit(2);
  }

  int nread = 0;
  while (argv[2][nread] != '\0') {
    if ((argv[2][nread] < '0') || (argv[2][nread] > '9')) {
      printf("Secondo argomento non intero\n");
      exit(2);
    }
    nread++;
  }
  port = atoi(argv[2]);
  if (port < 1024 || port > 65535) {
    printf("Porta scorretta...");
    exit(2);
  }

  servaddr.sin_addr.s_addr = ((struct in_addr *)(host->h_addr))->s_addr;
  servaddr.sin_port = htons(port);

  /* INIZIALIZZAZIONE INDIRIZZO CLIENT--------------------- */
  memset((char *)&clientaddr, 0, sizeof(struct sockaddr_in));
  clientaddr.sin_family = AF_INET;
  clientaddr.sin_addr.s_addr = INADDR_ANY;
  clientaddr.sin_port = 0;

  printf("Client avviato\n");

  /* CREAZIONE SOCKET ---------------------------- */
  sd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sd < 0) {
    perror("apertura socket");
    exit(3);
  }
  printf("Creata la socket sd=%d\n", sd);
  /* BIND SOCKET, a una porta scelta dal sistema --------------- */
  if (bind(sd, (struct sockaddr *)&clientaddr, sizeof(clientaddr)) < 0) {
    perror("bind socket ");
    exit(1);
  }
  printf("Client: bind socket ok, alla porta %i\n", clientaddr.sin_port);
#pragma endregion
  /*
  chiede ciclicamente all’utente il nome del file
  e la parola, invia al server la richiesta di eliminazione, e
  attende il pacchetto con l’esito dell’operazione, cioè il
  numero di eliminazioni della parola che stampa a video
  Il file è modificato nel servitore
  */
  do {
    printf("Nome del file: ");
    if (gets(nome_file) == 0) break;
    printf("Parola da eliminare: ");
    if (gets(parola) == 0) break;

    len = sizeof(servaddr);
    // nome_file|parola
    if (sendto(sd, strcat(strcat(nome_file, separator), parola),
               (strlen(nome_file) + strlen(parola) + 2), 0,
               (struct sockaddr *)&servaddr, len) < 0) {
      perror("sendto");
      continue;
    }

    if (recvfrom(sd, &result, sizeof(result), 0, (struct sockaddr *)&servaddr,
                 &len) < 0) {
      perror("recvfrom");
      continue;
    }

    if (result < 1)
      printf("Errore del server\n");
    else
      printf("Numero di parole eliminate %d\n", result);

  } while (1);

  printf("\nClient: termino...\n");
  shutdown(sd, 0);
  shutdown(sd, 1);
  close(sd);
  exit(0);
}
