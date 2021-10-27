/* Server che fornisce la valutazione di un'operazione tra due interi */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#define LINE_LENGTH 256

int main(int argc, char **argv)
{
	int sd, port, len, ris;
	const int on = 1;
	struct sockaddr_in cliaddr, servaddr;
	struct hostent *clienthost;
	char nomeFile[LINE_LENGTH];

	/* CONTROLLO ARGOMENTI ---------------------------------- */
	if (argc != 2)
	{
		printf("Error: %s port\n", argv[0]);
		exit(1);
	}
	port = atoi(argv[1]);
	if (port < 1024 || port > 65535)
	{
		printf("Error: %s port\n", argv[0]);
		printf("1024 <= port <= 65535\n");
		exit(2);
	}

	/* INIZIALIZZAZIONE INDIRIZZO SERVER ---------------------------------- */
	memset((char *)&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(port);

	/* CREAZIONE, SETAGGIO OPZIONI E CONNESSIONE SOCKET -------------------- */
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd < 0)
	{
		perror("creazione socket ");
		exit(1);
	}
	printf("Server: creata la socket, sd=%d\n", sd);

	if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
	{
		perror("set opzioni socket ");
		exit(1);
	}
	printf("Server: set opzioni socket ok\n");

	if (bind(sd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("bind socket ");
		exit(1);
	}
	printf("Server: bind socket ok\n");

	/* CICLO DI RICEZIONE RICHIESTE ------------------------------------------ */
	for (;;)
	{
		len = sizeof(struct sockaddr_in);
		if (recvfrom(sd, nomeFile, sizeof(nomeFile), 0, (struct sockaddr *)&cliaddr, &len) < 0)
		{
			perror("recvfrom ");
			continue;
		}

		clienthost = gethostbyaddr((char *)&cliaddr.sin_addr, sizeof(cliaddr.sin_addr), AF_INET);
		if (clienthost == NULL)
			printf("client host information not found\n");
		else
			printf("nome richiesto %s da: %s %i\n ", nomeFile, clienthost->h_name, (unsigned)ntohs(cliaddr.sin_port));

		//Esecuzione conteggio parola più lunga del file
		int fd;
		if ((fd = open(nomeFile, O_RDONLY)) < 0)
		{
			printf("errore apertura file");
			ris=-1;
		}
		else
		{
			int maxL = 0;
			int count = 0;
			char ch;
			while ((ch = read(fd, &ch, sizeof(char))) > 0)
			{
				if (ch != '\n' && ch != ' ')
				{
					count++;
				}
				else
				{
					if (maxL < count)
					{
						maxL = count;
					}
					count = 0;
				}
			}
			ris = maxL;
		}

		ris = htonl(ris);
		if (sendto(sd, &ris, sizeof(ris), 0, (struct sockaddr *)&cliaddr, len) < 0)
		{
			perror("sendto ");
			continue;
		}

		close(fd);

	} //for
}