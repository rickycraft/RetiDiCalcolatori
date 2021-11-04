/* Client per richiedere la lista di nomi di file remoti presenti 
nei direttori di secondo livello */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define DIM_BUFF 100
#define LENGTH_DIRECTORY_NAME 200
#define LENGTH_FILE_NAME 20

int main(int argc, char *argv[]){
	int sd, nread, port;
	char c, nome_direttorio[LENGTH_DIRECTORY_NAME], nome_file[LENGTH_FILE_NAME];
	struct hostent *host;
	struct sockaddr_in servaddr;  

	/* CONTROLLO NUMERO ARGOMENTI */
	if(argc!=3){
		printf("Error:%s serverAddress serverPort\n", argv[0]);
		exit(1);
	}
	printf("Client avviato\n");

	/* PREPARAZIONE INDIRIZZO SERVER */
	memset((char *)&servaddr, 0, sizeof(struct sockaddr_in));
	servaddr.sin_family = AF_INET;
	host = gethostbyname(argv[1]);
	/*GESTIONE ERRORE */
	if (host == NULL){
		printf("%s not found in /etc/hosts\n", argv[1]);
		exit(2);
	}

	/*CONTROLLO SECONDO ARGOMENTO: PORTA */
	nread = 0;
	while (argv[2][nread] != '\0'){
		if ((argv[2][nread] < '0') || (argv[2][nread] > '9')){
			printf("Secondo argomento non intero\n");
			exit(2);
		}
		nread++;
	}
	port = atoi(argv[2]);
	/*GESTIONE ERRORE*/
	if (port < 1024 || port > 65535)
	{printf("Porta scorretta...");exit(2);}

	servaddr.sin_addr.s_addr=((struct in_addr*) (host->h_addr))->s_addr;
	servaddr.sin_port = htons(port);

	/* CREAZIONE E CONNESSIONE SOCKET (BIND IMPLICITA)*/
	sd=socket(AF_INET, SOCK_STREAM, 0);
	if (sd <0){perror("apertura socket "); exit(3);}
	printf("Creata la socket sd=%d\n", sd);

	/* GESTIONE ERRORE */
	if (connect(sd,(struct sockaddr *) &servaddr, sizeof(struct sockaddr))<0)
	{perror("Errore in connect"); exit(4);}
	printf("Connect ok\n");

	/* CORPO DEL CLIENT */
	printf("Nome del direttorio: ");

	while (gets(nome_direttorio)){

		/*GESTIONE ERRORE INVIO*/
		if (write(sd, nome_direttorio, (strlen(nome_direttorio)+1))<0){
			perror("write");
			break;
		}
		printf("Richiesta del direttorio %s inviata... \n", nome_direttorio);
		int nread;
		char c;
		char sep='\n';
		printf("Ricevo la lista di nomi:\n");
		while((nread=read(sd,&c,sizeof(c)))>0)
		{
			if(c!=';')
			{
				write(1,&c,1);
			}
			else
			{
				write(1,&sep,1);
			}
			
		}
		if(nread<0)
		{
			perror("read nomeFIle");
			break;
		}
		
		

		printf("Nome del direttorio: ");
	}//while
	printf("\nClient: termino...\n");
	shutdown(sd,0);
	shutdown(sd,1);
	close(sd);
	exit(0);
}
