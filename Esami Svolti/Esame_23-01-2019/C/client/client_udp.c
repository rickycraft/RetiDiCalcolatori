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
  char result[DIM_RESULT];

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
  printf("Per aggiornare patente prenotazione.\nInserisci la targa: ");
  while (gets(tmp))
  {

    
    if (strlen(tmp) == 7)
    {
      char *targa=malloc(strlen(tmp)+2);
      strcpy(targa,tmp);
      printf("targa inserita:%s",targa);
      //correttamente inserita.
      //Si potrebbe pure contorllare che ci siano 3 cifre e 4 caratteri ma non so se tutte le targhe sono così.
      printf("Inserisci la patente");
      gets(tmp);
      int corretto = 1;
      if (strlen(tmp) == 5)
      {
        for (int i = 0; i < 5; i++)
        {
          if ((tmp[i] < '0') || (tmp[i] > '9'))
          {
            corretto = 0;
          }
        }
        if (corretto == 0)
        {
          printf("La patente è formata da soli numeri.\n");
        }
        else
        {
          strcat(tmp," ");
          strcat(tmp,targa);
          //tutto ok e mando
          len = sizeof(servaddr);
          if (sendto(sd, tmp, strlen(tmp) + 1, 0, (struct sockaddr *)&servaddr, len) <
              0)
          {
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
            if(strcmp(result,"OK")==0){
              printf("L'operazione è riuscita con successo!");
            }else{
              printf("L'operazione di aggiornamento non è riuscita!");
            }
          }
        }
      }
      else
      {
        printf("La patente è formata da 5 numeri.");
      }
    }
    else
    {
      printf("Hai inserito una targa mal formata.\n");
    }

    /* invio richiesta */

      printf("Inserisci la patente");

  } // while

  printf("\nclient# killed\n");
  shutdown(sd, 0);
  shutdown(sd, 1);
  close(sd);
  exit(0);
}
