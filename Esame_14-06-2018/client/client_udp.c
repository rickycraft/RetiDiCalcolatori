/* Client per richiedere il numero di file in un direttorio remoto */

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

#define DIM_BUFF 100
#define DIM_RESULT 32

int main(int argc, char **argv)
{
  struct hostent *host;
  struct sockaddr_in clientaddr, servaddr;
  int sd, nread, port, len;
  char tmp[DIM_BUFF];
  int result;

  /* CONTROLLO ARGOMENTI ---------------------------------- */
  if (argc != 3)
  {
    printf("client# Error: argc != 3\n");
    exit(1);
  }

  /* INIZIALIZZAZIONE INDIRIZZO SERVER--------------------- */
  memset((char *)&servaddr, 0, sizeof(struct sockaddr_in));
  servaddr.sin_family = AF_INET;
  host = gethostbyname(argv[1]);
  if (host == NULL)
  {
    printf("client# %s not found in /etc/hosts\n", argv[1]);
    exit(2);
  }

  nread = 0;
  while (argv[2][nread] != '\0')
  {
    if ((argv[2][nread] < '0') || (argv[2][nread] > '9'))
    {
      printf("client# secondo argomento non intero\n");
      exit(2);
    }
    nread++;
  }
  port = atoi(argv[2]);
  if (port < 1024 || port > 65535)
  {
    printf("client# porta scorretta...");
    exit(2);
  }

  servaddr.sin_addr.s_addr = ((struct in_addr *)(host->h_addr_list[0]))->s_addr;
  servaddr.sin_port = htons(port);

  /* INIZIALIZZAZIONE INDIRIZZO CLIENT--------------------- */
  memset((char *)&clientaddr, 0, sizeof(struct sockaddr_in));
  clientaddr.sin_family = AF_INET;
  clientaddr.sin_addr.s_addr = INADDR_ANY;
  clientaddr.sin_port = 0;

  printf("client# avviato\n");

  /* CREAZIONE SOCKET ---------------------------- */
  sd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sd < 0)
  {
    perror("apertura socket");
    exit(3);
  }
  printf("client# creata la socket sd=%d\n", sd);

  /* BIND SOCKET, a una porta scelta dal sistema --------------- */
  if (bind(sd, (struct sockaddr *)&clientaddr, sizeof(clientaddr)) < 0)
  {
    perror("bind socket ");
    exit(1);
  }
  printf("client# bind socket, porta %i\n", clientaddr.sin_port);

  /* CORPO DEL CLIENT: */
  printf("Inserisci il nome del file remoto: ");
  while (gets(tmp))
  {

    len = sizeof(servaddr);
    if (sendto(sd, tmp, strlen(tmp) + 1, 0, (struct sockaddr *)&servaddr, len) <
        0)
    {
      printf("errore nella send\n");
      continue;
    }

    if (recvfrom(sd, result, sizeof(result), 0, (struct sockaddr *)&servaddr, &len) <
        0)
    {
      perror("recvfrom");
      continue;
    }
    else
    {

      if (result >= 0)
      {
        printf("L'operazione è riuscita con successo! con %d occorrenze", result);
      }
      else
      {
        printf("L'operazione di aggiornamento non è riuscita!");
      }
    }

    printf("Inserisci il nome del file remoto: ");

  } // while

  printf("\nclient# killed\n");
  shutdown(sd, 0);
  shutdown(sd, 1);
  close(sd);
  exit(0);
}
