
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
	int *risAggiunta;
	int *risAcquisto;
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
	printf("Inserire: A per aggiornare la patente \nInserire: V per visualizzare la lista.\n");

	while (gets(buffer))
	{

		//se operazione aggiungere evento
		if (strcmp(buffer, "A") == 0)
		{
			//ora deve inserire i dati dell'evento.
			input = (Input *)malloc(sizeof(Input));
			while (correttoInserimento == 0)
			{
				printf("Inserisci la targa:\n");
				gets(buffer);

				if (strlen(buffer) != 7)
				{
					printf("Devi inserire correttamente la targa\n");
					correttoInserimento = 0;
				}
				else{
					correttoInserimento = 1;
				}
			}
			printf("targa:%s\n",buffer);
			strcpy(input->targa, buffer);
			//input->targa[7]='\0';

			correttoInserimento = 0;
			while (!correttoInserimento)
			{
				printf("Inserisci la patente\n");
				gets(buffer);
				if (strlen(buffer) != 5)
				{
					printf("Inserisci correttamente la patente\n");
				}
				else
				{
					for (int i = 0; i < strlen(buffer); i++)
					{
						if (buffer[i] < '0' || buffer[i] > '9')
						{
							errore = 1;
							printf("non è formato da soli numeri\n");
							break;
						}
					}
					if (errore == 0)
					{
						correttoInserimento = 1;
					}
				}
			}
			
			printf("patente:%s\n",buffer);
			strcpy(input->patente, buffer);
			//input->patente[5]='\0';
			printf("quindi spedisco al server:\n Targa:%s\t patente:%s\n", input->targa, input->patente);
			risAggiunta = aggiorna_licenza_1(input, cl);

			//Controllo del risultato
			if (risAggiunta == NULL)
			{
				//Errore di RPC
				clnt_perror(cl, host);
				exit(1);
			}

			if (*risAggiunta == 1)
			{
				printf("Operazione di aggiunta riuscita con successo!\n");
			}
			if (*risAggiunta == -1)
			{
				printf("L'aggiunta dell'evento non è riuscita");
			}

		} // if 1
		  //ser operazione 2
		else if (strcmp(buffer, "V") == 0)
		{
			printf("\nInserisci il tipo di mezzo: \n");
			gets(buffer);
			correttoInserimento = 0;
			while (correttoInserimento == 0)
			{
				if(strcmp(buffer,"auto")!=0 && strcmp(buffer,"camper")!=0){
					//errore
					printf("devi inserire o auto o camper\n");
				}else{
					correttoInserimento=1;
				}

			}

			// Invocazione remota
			
			e = visualizza_prenotazioni_1(&buffer, cl);

			//Controllo del risultato
			if (e == NULL)
			{
				//Errore di RPC
				clnt_perror(cl, host);
				exit(1);
			}

			for (int i = 0; i < MAX_PRENOTAZIONI; i++)
			{

        	if (strcmp(e->prenotazioni[i].targa, "L") != 0)
        	{
        	  printf("targa :%s \tPatente:%s\tTipo:%s\n", (e->prenotazioni[i].targa),e->prenotazioni[i].patente,e->prenotazioni[i].tipo);
        	}
     		}


		} // if 2
		else
			printf("[ERROR]Operazione richiesta non disponibile!!\n");

	printf("Inserire: A per aggiornare la patente \nInserire: V per visualizzare la lista.\n");

	} // while

	// Libero le risorse, distruggendo il gestore di trasporto
	clnt_destroy(cl);
	free(input);
	free(e);
	printf("TERMINO ");
	exit(0);
} //main
