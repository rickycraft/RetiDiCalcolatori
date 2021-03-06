/* Client per richiedere l'ordinamento remoto di un file */

#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define DIM_BUFF 256
#define LENGTH 100

int main(int argc, char *argv[])
{
	int sd, port, fd_sorg, fd_dest, nread;
	char buff[DIM_BUFF];
	// FILENAME_MAX: lunghezza massima nome file. Costante di sistema.
	char nome_sorg[FILENAME_MAX + 1], nome_dest[FILENAME_MAX + 1];
	struct hostent *host;
	struct sockaddr_in servaddr;

	/* CONTROLLO ARGOMENTI ---------------------------------- */
	if (argc != 3)
	{
		printf("Error:%s serverAddress serverPort\n", argv[0]);
		exit(1);
	}

	/* INIZIALIZZAZIONE INDIRIZZO SERVER -------------------------- */
	memset((char *)&servaddr, 0, sizeof(struct sockaddr_in));
	servaddr.sin_family = AF_INET;
	host = gethostbyname(argv[1]);

	/*VERIFICA INTERO*/

	port = atoi(argv[2]);

	/* VERIFICA PORT e HOST */
	if (port < 1024 || port > 65535)
	{
		printf("%s = porta scorretta...\n", argv[2]);
		exit(2);
	}
	if (host == NULL)
	{
		printf("%s not found in /etc/hosts\n", argv[1]);
		exit(2);
	}
	else
	{
		servaddr.sin_addr.s_addr = ((struct in_addr *)(host->h_addr))->s_addr;
		servaddr.sin_port = htons(port);
	}

	/* CORPO DEL CLIENT:
	ciclo di accettazione di richieste da utente ------- */
	printf("Inserire nome del file: \n");

	while (gets(nome_sorg))
	{
		printf("File da aprire: %s\n", nome_sorg);
		nome_sorg[FILENAME_MAX] = '\0';
		/* Verifico l'esistenza del file */
		if ((fd_sorg = open(nome_sorg, O_RDONLY)) < 0)
		{
			perror("open file sorgente");
			printf("Qualsiasi tasto per procedere, EOF per fine: \n");
			continue;
		}
		char valore[LENGTH];
		printf("Riga da eliminare (considerando che la prima riga sia la 1)\n ");
		gets(valore);
		int val = atoi(valore);
		if (val < 1)
		{
			printf("Hai inserito una riga non corretta");
			exit(-1);
		}

		/*Verifico creazione file*/
		if ((fd_dest = open(nome_sorg, O_WRONLY, 0644)) < 0)
		{
			perror("open file destinatario");
			printf("Nome del file da ordinare, EOF per terminare: ");
			continue;
		}

		/* CREAZIONE SOCKET ------------------------------------ */
		sd = socket(AF_INET, SOCK_STREAM, 0);
		if (sd < 0)
		{
			perror("apertura socket");
			exit(1);
		}
		printf("Client: creata la socket sd=%d\n", sd);

		/* Operazione di BIND implicita nella connect */
		if (connect(sd, (struct sockaddr *)&servaddr, sizeof(struct sockaddr)) < 0)
		{
			perror("connect");
			exit(1);
		}
		printf("Client: connect ok\n");

		/*INVIO File*/
		//printf("Invio nome file %d\n", write(sd, nome_sorg, FILENAME_MAX));
		printf("Client: invio riga da eliminare\n");
		//val = htons(val);
		write(sd, &val, sizeof(int));

		printf("Client: invio contenuto del file\n");
		while ((nread = read(fd_sorg, buff, DIM_BUFF)) > 0)
		{
			//stampa
			write(sd, &buff, nread); //invio
		}
		printf("Client: file inviato\n");
		/* Chiusura socket in output/scrittura -> invio dell'EOF */
		shutdown(sd, 1);
		close(fd_sorg); // chiusura file in lettura

		
		/*RICEZIONE File*/
		printf("Client: ricevo e stampo file\n");
		while ((nread = read(sd, buff, DIM_BUFF)) > 0)
		{
			write(fd_dest, buff, nread);
			write(1, buff, nread);
		}
		printf("Trasferimento terminato\n");
		/* Chiusura socket in ricezione */
		shutdown(sd, 0);
		/* Chiusura file */
		
		close(fd_dest);
		close(sd); //chiudo socket

		printf("Nome del file da ordinare, EOF per terminare: ");
	} //while
	printf("\nClient: termino...\n");
	exit(0);
}
