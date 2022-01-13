/* Client per richiedere l'invio di un file (get, versione 1) */

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
#define DIM_RESULT 512

int main(int argc, char *argv[])
{
  int sd, nread, port;
  char tmp[DIM_BUFF];
  char result[DIM_RESULT];
  struct hostent *host;
  struct sockaddr_in servaddr;
#pragma region setup
  /* CONTROLLO ARGOMENTI ---------------------------------- */
  if (argc != 3)
  {
    printf("client# Error: argc != 3\n");
    exit(1);
  }
  printf("client# Client avviato\n");

  /* PREPARAZIONE INDIRIZZO SERVER ----------------------------- */
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

  /* CREAZIONE E CONNESSIONE SOCKET (BIND IMPLICITA) ----------------- */
  sd = socket(AF_INET, SOCK_STREAM, 0);
  if (sd < 0)
  {
    perror("apertura socket ");
    exit(3);
  }
  printf("client# creata la socket sd=%d\n", sd);

  if (connect(sd, (struct sockaddr *)&servaddr, sizeof(struct sockaddr)) < 0)
  {
    perror("Errore in connect");
    exit(4);
  }
  printf("client# connect ok\n");


  /* CORPO DEL CLIENT: */
  printf("Inserisci la targa per ricevere le foto: ");
  while (gets(tmp))
  {
    if (strlen(tmp) == 7)
    {
     
      if (write(sd,tmp, strlen(tmp)+1) < 0)
      {
        perror("write");
        break;
      }
      printf("invio di %s\n",tmp);
      char c;
      char nomeFile[100];
      int i = 0;
      // trasmissioni così formata |nomeFile*lunghezzaFile| DATI
      long lunghezzaFile;
      int fileRimanente;
      int info = 0;
      int fd;
          
      char *dir=malloc(strlen(tmp)+strlen("./")+1);
      strcpy(dir,"./");
      strcpy(dir,tmp);
      mkdir(strcat(tmp,"_img"));
      
      printf("Cartella creatan");
      chdir(dir);
      while (read(sd, &c, 1) > 0)
      {
        //parte info
        if (c == '|')
        {
          info++;
        }
        else
        {
          strcat(tmp, c);
        }

        if (info == 2)
        {
          //arrivo dati.
          info = 0;
          char *copytmp = malloc(strlen(tmp) + 1);
          strcpy(copytmp, tmp);
          char *nome = strtok(copytmp, "*");
          strcpy(nomeFile, nome);
          char *length = strtok(NULL, "*");
          lunghezzaFile = atoi(length);
          int carLetti = 0;
          
          printf("ricevuto nomeFile:%s \t lunghezzaFile %ld\n",nomeFile,lunghezzaFile);

          fd = open(nomeFile, O_WRONLY | O_CREAT);
          fileRimanente = lunghezzaFile;
          for (i = 0; fileRimanente>0; i++)
          {
            if (fileRimanente < 64)
            {
              carLetti = read(sd, result, fileRimanente);
              fileRimanente=0;
            }
            else
            {
              carLetti = read(sd, result, 64);
              fileRimanente-=64;
            }
            write(fd, result, carLetti); //scrivo su file
           
          }
           printf("Ho ricevuto %s \n",nomeFile);
           close(fd);
        }
      }
    }
    else
    {
      printf("Inserisci correttamente la targa\n");
    }

   
    printf("Inserisci la targa per ricevere le foto: ");
  }

  printf("client# killed\n");
  shutdown(sd, 0);
  shutdown(sd, 1);
  close(sd);
  exit(0);
}
