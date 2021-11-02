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
#define max(a, b) ((a) > (b) ? (a) : (b))

/********************************************************/
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
/********************************************************/
void gestore(int signo)
{
	int stato;
	printf("esecuzione gestore di SIGCHLD\n");
	wait(&stato);
}
/********************************************************/

int main(int argc, char **argv)
{
	int listenfd, connfd, udpfd, fd_file, nready, maxfdp1;
	const int on = 1;
	char zero = 0, buff[DIM_BUFF], datagram_in[2 * LENGTH_FILE_NAME], nome_directory[LENGTH_FILE_NAME];
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

	/* INIZIALIZZAZIONE INDIRIZZO SERVER ----------------------------------------- */
	memset((char *)&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(port);

	printf("Server avviato\n");

	/* CREAZIONE SOCKET TCP ------------------------------------------------------ */
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

	/* AGGANCIO GESTORE PER EVITARE FIGLI ZOMBIE -------------------------------- */
	signal(SIGCHLD, gestore);
#pragma endregion controlli
	/* PULIZIA E SETTAGGIO MASCHERA DEI FILE DESCRIPTOR ------------------------- */
	FD_ZERO(&rset);
	maxfdp1 = max(listenfd, udpfd) + 1;

	/* CICLO DI RICEZIONE EVENTI DALLA SELECT ----------------------------------- */
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
			restituisce il nome dei file contenuti in tutti i direttori di secondo livello
			in caso di errore notifica il cliente ma tiene la connessione aperta
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
					if ((read(connfd, &nome_directory, sizeof(nome_directory))) <= 0)
					{
						perror("read");
						break;
					}
					printf("Richiesta directory %s\n", nome_directory);

					// leggo i file e mando il nome dei file, ho la connessione

					printf("Terminato invio nomi file\n");

					/* 
					invio al client segnale di terminazione: zero binario
					write(connfd, &zero, 1);
					close(fd_file);
					non serve
					*/
				}
			}
			printf("Figlio %i: chiudo connessione e termino\n", getpid());
			close(connfd);
			exit(0);
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
			if (recvfrom(udpfd, &datagram_in, sizeof(datagram_in), 0, (struct sockaddr *)&cliaddr, &len) < 0)
			{
				perror("recvfrom");
				continue;
			}

			printf("Ricevuto datagram %s\n", datagram_in);
			// split datagram_in nomefile|parola

			// num = eliminaparoleinfile(nomefile, parola)

			printf("Risultato del conteggio: %i\n", num);
			ris = htonl(num);
			if (sendto(udpfd, &ris, sizeof(ris), 0, (struct sockaddr *)&cliaddr, len) < 0)
			{
				perror("sendto");
				continue;
			}

		} /* fine gestione richieste di conteggio */
	}		/* ciclo for della select */
}
