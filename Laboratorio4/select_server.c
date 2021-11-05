#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define DIM_BUFF 100
#define LENGTH_FILE_NAME 20
#define LENGHT_WORD 32
#define LENGTH_DIRECTORY_NAME 200
#define max(a, b) ((a) > (b) ? (a) : (b))

int conta_file(char *name)
{
  DIR *dir;
  struct dirent *dd;
  int count = 0;
  dir = opendir(name);
  if (dir == NULL)
    return -1;
  while ((dd = readdir(dir)) != NULL)
  {
    printf("Trovato il file %s\n", dd->d_name);
    count++;
  }
  printf("Numero totale di file %d\n", count);
  closedir(dir);
  return count;
}

int elimina_parole_dafile(char *nome_file, char *parola) { 
    int fd_in,fd_out;
    char nomeTemp[LENGTH_FILE_NAME+5];
    char c;
    char temp[LENGHT_WORD];
    int i=0;
    int count=0;
    if((fd_in=open(nome_file,O_RDONLY))<0){
    printf("errore dell aperture file in lettura");
    return -1;
    }
    strcpy(nomeTemp,nome_file);
    strcat(nomeTemp,"_temp");
    printf("aperto file in lettura\n");
    if((fd_out=open(nomeTemp,O_CREAT|O_WRONLY,0777))<0){
    printf("errore dell aperture/creazione file temporanea %s", nomeTemp);
    return -1;
    }
    printf("aperto file in scrittura \n");
    /*
    leggo carattere per carattere e salvo in una stringa temporanea 
    quando leggo uno spazio confronto temp con la parola 
    se è la parola count++ altrimenti scrivo temp e spazio
    */
    while(read(fd_in,&c,1)>0){
        if(c!=' ' && c!='\n' && c!='\t'){
            temp[i]=c;
            i++;
        }else {
            if(strcmp(parola,temp)==0){
                count++;
                printf("eliminata %s\n",parola);
            }else {
                write(fd_out,temp,i);
                printf("%s\n",temp);
            }
            write(fd_out,&c,1);    
            memset(temp, 0 , i);
            i=0;
        }
    }//ultima parola prima del EOF
    if(strcmp(parola,temp)==0)
        count++;
    else 
        write(fd_out,temp,i);

    printf("count %d \n",count );
    close(fd_in);
    close(fd_out);

    remove(nome_file);
    rename(nomeTemp,nome_file);

    return count;

 }

void gestore(int signo)
{
  int stato;
  printf("esecuzione gestore di SIGCHLD\n");
  wait(&stato);
}

int main(int argc, char **argv)
{
  int listenfd, connfd, udpfd, fd_file, nready, maxfdp1;
  const int on = 1;
  char buff[DIM_BUFF], packet_in[2 * LENGTH_FILE_NAME];
  char *nome_file, *parola;
  const char *separator = "|", *current_dir = ".";
  DIR *dir, *subdir;
  struct dirent *dd, *dd2;
  fd_set rset;
  int len, nread, nwrite, num, ris, port;
  struct sockaddr_in cliaddr, servaddr;
#pragma region controlli
  /* CONTROLLO ARGOMENTI ---------------------------------- */
  if (argc != 2)
  {
    printf("Error: %s port\n", argv[0]);
    exit(1);
  }
  nread = 0;
  while (argv[1][nread] != '\0')
  {
    if ((argv[1][nread] < '0') || (argv[1][nread] > '9'))
    {
      printf("Terzo argomento non intero\n");
      exit(2);
    }
    nread++;
  }
  port = atoi(argv[1]);
  if (port < 1024 || port > 65535)
  {
    printf("Porta scorretta...");
    exit(2);
  }

  /* INIZIALIZZAZIONE INDIRIZZO SERVER -----------------------------------------
   */
  memset((char *)&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(port);

  printf("Server avviato\n");

  /* CREAZIONE SOCKET TCP ------------------------------------------------------
   */
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  if (listenfd < 0)
  {
    perror("apertura socket TCP ");
    exit(1);
  }
  printf("Creata la socket TCP d'ascolto, fd=%d\n", listenfd);

  if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
  {
    perror("set opzioni socket TCP");
    exit(2);
  }
  printf("Set opzioni socket TCP ok\n");

  if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
  {
    perror("bind socket TCP");
    exit(3);
  }
  printf("Bind socket TCP ok\n");

  if (listen(listenfd, 5) < 0)
  {
    perror("listen");
    exit(4);
  }
  printf("Listen ok\n");

  /* CREAZIONE SOCKET UDP ------------------------------------------------ */
  udpfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (udpfd < 0)
  {
    perror("apertura socket UDP");
    exit(5);
  }
  printf("Creata la socket UDP, fd=%d\n", udpfd);

  if (setsockopt(udpfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
  {
    perror("set opzioni socket UDP");
    exit(6);
  }
  printf("Set opzioni socket UDP ok\n");

  if (bind(udpfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
  {
    perror("bind socket UDP");
    exit(7);
  }
  printf("Bind socket UDP ok\n");

  /* AGGANCIO GESTORE PER EVITARE FIGLI ZOMBIE --------------------------------
   */
  signal(SIGCHLD, gestore);
#pragma endregion controlli
  /* PULIZIA E SETTAGGIO MASCHERA DEI FILE DESCRIPTOR --------------------*/
  FD_ZERO(&rset);
  maxfdp1 = max(listenfd, udpfd) + 1;

  /* CICLO DI RICEZIONE EVENTI DALLA SELECT -------------------------------*/
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

    /* GESTIONE TCP ------------------------------------- */
    if (FD_ISSET(listenfd, &rset))
    {
      /*
      Il server riceve il nome del direttorio; se il direttorio esiste
      restituisce il nome dei file contenuti in tutti i direttori di secondo
      livello in caso di errore notifica il cliente
      */
      printf("Ricevuta richiesta\n");
      len = sizeof(struct sockaddr_in);
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
      if (fork() == 0)
      {
        close(listenfd);
        printf("Dentro il figlio, pid=%i\n", getpid());

        for (;;)
        {
          if ((read(connfd, &packet_in, sizeof(packet_in))) <= 0)
          {
            perror("read");
            break;
          }
          printf("Richiesta directory %s\n", packet_in);
          // non funziona nella seconda
          if (chdir(packet_in) < 0)
          {
            // gestione dell'errore da migliorare
            break;
          }

          dir = opendir(current_dir);
          while ((dd = readdir(dir)) != NULL)
          {
            // se dd è una directory
            if (dd->d_type == DT_DIR)
            {
              printf("Subdirectory: %s\n", (dd->d_name));
              subdir = opendir(dd->d_name);
              while ((dd2 = readdir(subdir)) != NULL)
              {
                if (dd2->d_type == DT_REG)
                {
                  printf("Trovato il file %s\n", dd2->d_name);
                  write(connfd, dd2->d_name, sizeof(dd2->d_name));
                }
              }
              closedir(subdir);
            }
          }
          write(connfd, separator, sizeof(char));
          closedir(dir);
          printf("Terminato invio nomi file\n");
        }
        printf("Figlio %i: chiudo connessione e termino\n", getpid());
        close(connfd);
        exit(0);
      }
      close(connfd);
    }

    /* GESTIONE UDP ------------------------------------------ */
    if (FD_ISSET(udpfd, &rset))
    {
      /*
      Il server riceve il datagramma con il nome del file e la parola:
      se il file esiste, elimina le occorrenze della parola all’interno del
      file e invia al client l’intero positivo corrispondente alle eliminazioni
      fatte in caso di errore, invia un intero negativo
      */
      printf("Server: ricevuta richiesta di conteggio file\n");
      len = sizeof(struct sockaddr_in);
      num = -1;
      if (recvfrom(udpfd, &packet_in, sizeof(packet_in), 0,
                   (struct sockaddr *)&cliaddr, &len) < 0)
      {
        perror("recvfrom");
        continue;
      }

      nome_file = strtok(packet_in, separator);
      parola = strtok(NULL, separator);
      printf("Ricevuto datagram %s|%s\n", nome_file, parola);
      num = elimina_parole_dafile(nome_file, parola);
      printf("Risultato del conteggio: %i\n", num);
      if (sendto(udpfd, &num, sizeof(num), 0, (struct sockaddr *)&cliaddr,
                 len) < 0)
      {
        perror("sendto");
        continue;
      }
    }
  }
}
