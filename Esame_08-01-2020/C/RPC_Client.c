
#include <rpc/rpc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "RPC_xFile.h"

#define DIM 16
//

int main(int argc, char *argv[])
{
	char *host; //nome host
	CLIENT *cl; //gestore del trasporto
	char *buffer;
	buffer=malloc(DIM);
	//dichiarazioni delle variabili
	int correttoInserimento = 0;
	int errore = 0;
	int *risElimina;
	Output *e;
	//void *voidValue;
	Input *input;
	

	/****************************************************************************************************************/

	//CONTROLLO DEGLI ARGOMENTI
	if (argc != 2)
	{
		printf("[ERROR] usage: %s server_host\n", argv[0]);
		exit(1);
	}
	host = argv[1];

	//CREAZIONE GESTORE DI TRASPORTO
	cl = clnt_create(host, RPCESAME, RPCESAMEVERS, "udp");
	if (cl == NULL)
	{
		clnt_pcreateerror(host);
		exit(1);
	}

	//INTERAZIONE CON L UTENTE
	printf("Inserire: E per eliminare occorrenze\nInserire: L per listaFileCarattere\n");

	while (gets(buffer))
	{

		//se operazione aggiungere evento
		if (strcmp(buffer, "E") == 0)
		{
			//ora deve inserire i dati dell'evento.
			printf("Inserisci il nome del File");
			gets(buffer);
			
			risElimina =elimina_occorrenze_1(&buffer, cl);

			//Controllo del risultato
			if (risElimina == NULL)
			{
				//Errore di RPC
				printf("ERRORE LATO SERVER\n");
				clnt_perror(cl, host);
				exit(1);
			}

			if (*risElimina >0)
			{
				printf("Operazione di aggiunta riuscita con successo!\n");
				printf("eliminate %d occorrenze",*risElimina);
			}else{
				printf("L'operazione non è riuscita");
			}
			

		} // if 1
		  //ser operazione 2
		else if (strcmp(buffer, "L") == 0)
		{
			printf("\nInserisci la directory: \n");
			gets(buffer);
			input=(Input*) malloc(sizeof(Input));
			input->nomeDir=malloc(strlen(buffer)+1);
			strcpy(input->nomeDir,buffer);
			input->nomeDir[strlen(buffer)]='\0';

			printf("Inserisci il carattere");
			gets(buffer);
			input->carattere=buffer[0];
			
			printf("Inserisci il numero di occorrenze minimo");
			gets(buffer);

			correttoInserimento=0;
			while(!correttoInserimento){
				
				for(int i=0;i<strlen(buffer);i++){
					if(buffer[i]<'0' ||buffer[i]>'9'){
						errore=1;
						printf("devi scrivere un numero. Riprova\n");
						gets(buffer);
					}
				}
				if(errore==0){
					correttoInserimento=1;
				}
				
			}
			input->numOccorrenze=atoi(buffer);

			// Invocazione remota
			e = lista_file_carattere_1(input, cl);
			//Controllo del risultato
			if (e == NULL)
			{
				//Errore di RPC
				clnt_perror(cl, host);
				exit(1);
			}
			for (int i = 0; i < e->indice; i++)
			{
				printf("nome:%s\n",e->listaNomeFile[i].nomeFile);
        	}
		} // if 2
		else
			printf("[ERROR]Operazione richiesta non disponibile!!\n");

	printf("Inserire: E per eliminare occorrenze\nInserire: L per listaFileCarattere\n");

	} // while

	// Libero le risorse, distruggendo il gestore di trasporto
	clnt_destroy(cl);
	free(input);
	free(e);
	printf("TERMINO ");
	exit(0);
} //main
