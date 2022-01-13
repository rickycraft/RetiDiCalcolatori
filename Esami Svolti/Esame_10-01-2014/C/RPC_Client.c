
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
	char buffer[DIM];
	//dichiarazioni delle variabili
	int correttoInserimento = 0;
	int errore = 0;
	int *risAggiunta;
	int *risAcquisto;
	Evento *e;
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
	printf("Inserire: i per inserire un evento \nInserisci: a per acquistare dei biglietti.\n");

	while (gets(buffer))
	{

		//se operazione aggiungere evento
		if (strcmp(buffer, "i") == 0)
		{
			//ora deve inserire i dati dell'evento.
			printf("Inserisci il nome dell'evento:\n");
			gets(buffer);
			e = (Evento *)malloc(sizeof(Evento));

			e->id = malloc(strlen(buffer) + 1);
			strcpy(e->id, buffer);
			printf("Bene, adesso inserisci il tipo di Evento(tra Concerto, Calcio e Formula1):\n");
			gets(buffer);
			while (correttoInserimento == 0)
			{
				if (strcmp(buffer, "Concerto") == 0)
				{
					//Ok
					correttoInserimento = 1;
					e->tipo = malloc(sizeof(char) * strlen(buffer) + 1);
					strcpy(e->tipo, buffer);
				}
				else if (strcmp(buffer, "Calcio") == 0)
				{
					correttoInserimento = 1;

					e->tipo = malloc(sizeof(char) * strlen(buffer) + 1);
					strcpy(e->tipo, buffer);
				}
				else if (strcmp(buffer, "Formula1") == 0)
				{
					correttoInserimento = 1;

					e->tipo = malloc(sizeof(char) * strlen(buffer) + 1);
					strcpy(e->tipo, buffer);
				}
				if (correttoInserimento == 0)
				{
					printf("Inserisci correttamente la tipologia. Seleziona tra Concerto, Calcio e Formula1\n");
					gets(buffer);
				}
			}

			printf("Inserisci il luogo:\n");
			gets(buffer);
			e->luogo = malloc(sizeof(char) * strlen(buffer) + 1);
			strcpy(e->luogo, buffer);
			printf("Inserisci la data (gg/mm/aaaa), il numero di biglietti disponibili e il prezzo\n");
			gets(buffer);
			correttoInserimento = 0;
			while (correttoInserimento == 0)
			{
				char *stringaDaSeparare = malloc(strlen(buffer) + 1);

				strcpy(stringaDaSeparare, buffer);
				char *data = strtok(stringaDaSeparare, " ");
				printf("data ricevuta %s\n", data);
				char *biglietti = strtok(NULL, " ");
				printf("biglietti ricevuti %s\n", biglietti);
				char *prezzo = strtok(NULL, " ");

				printf("prezzo ricevuto %s\n", prezzo);
				if (strlen(data) != 10)
				{
					printf("DEVI INSERIRE CORRETTAMENTE LA DATA.\n");
					errore = 1;
				}
				else
				{
					char *anno = malloc(sizeof(char) * 5);
					char *mese = malloc(sizeof(char) * 3);
					char *giorno = malloc(sizeof(char) * 3);
					strncpy(giorno, data, 2);
					giorno[2] = '\0'; //Ricordarsi lo \0 altrimenti non termina la stringa
					strncpy(mese, data + 3, 2);
					mese[2] = '\0';
					strncpy(anno, data + 6, 4);
					anno[4] = '\0';
					printf("giorno Splittato:%s\n", giorno);
					printf("mese Splittato:%s\n", mese);
					printf("anno Splittato:%s\n", anno);
					for (int i = 0; i < strlen(anno); i++)
					{
						if (i < 2)
						{
							if (giorno[i] < '0' || giorno[i] > '9')
							{
								//char diverso da numero;
								errore = 1;
								printf("errore giorno: %c", giorno[i]);
								break;
							}
							if (mese[i] < '0' || mese[i] > '9')
							{
								errore = 1;

								printf("errore mese: %c", mese[i]);
								break;
							}
						}
						if (anno[i] < '0' || anno[i] > '9')
						{
							errore = 1;

							printf("errore anno: %c", anno[i]);
							break;
						}
					}
					free(anno);
					free(mese);
					free(giorno);

					if (errore == 0)
					{
						printf("data scritta correttamente\n");

						for (int i = 0; i < strlen(biglietti); i++)
						{
							if (biglietti[i] < '0' || biglietti[i] > '9')
							{
								errore = 1;

								printf("errore biglietto: %c", biglietti[i]);
								break;
							}
						}
						for (int i = 0; i < strlen(prezzo); i++)
						{
							if (prezzo[i] < '0' || prezzo[i] > '9')
							{
								errore = 1;

								printf("errore prezzo: %c", prezzo[i]);
								break;
							}
						}
					}
				}

				if (errore == 0)
				{
					correttoInserimento = 1;
					e->data = malloc(sizeof(char) * strlen(data) + 1);
					strcpy(e->data, data);

					e->prezzo = atoi(prezzo);

					e->bigliettiDisponibili = atoi(biglietti);

					//tutto corretto
				}
				else
				{
					printf("Hai sbagliato l'inserimento dei dati. Riinseriscili\n");
					gets(buffer);
				}
			}
			//ho chiesto tutto.

			printf("quindi spedisco al server:\n ID:%s\nTIPO:%s\nLUOGO:%s\nDATA: %s\nBigliettiDisponibili: %i\nPrezzo: %i\n", e->id, e->tipo, e->luogo, e->data, e->bigliettiDisponibili, e->prezzo);
			risAggiunta = inserimento_evento_1(e, cl);

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
		else if (strcmp(buffer, "a") == 0)
		{
			input = (Input *)malloc(sizeof(Input));

			printf("\nInserisci l'id dell'evento: \n");
			gets(buffer);
			input->id = malloc(sizeof(buffer));
			strcpy(input->id, buffer);
			printf("id:%s\n", input->id);

			printf("\nInserisci il numero di biglietti: \n");
			gets(buffer);
			correttoInserimento = 0;
			while (correttoInserimento == 0)
			{

				for (int i = 0; i < strlen(buffer); i++)
				{
					if (buffer[i] < '0' || buffer[i] > '9')
					{
						errore = 1;
						break;
					}
				}
				if (errore == 0)
				{
					correttoInserimento = 1;
					input->numBigliettiDaAcquistare = atoi(buffer);
				}
			}

			// Invocazione remota
			risAcquisto = acquista_biglietti_1(input, cl);

			//Controllo del risultato
			if (risAcquisto == NULL)
			{
				//Errore di RPC
				clnt_perror(cl, host);
				exit(1);
			}

			if (*risAcquisto < 0)
				//Eventuale errore di logica del programma
				printf("[ERROR] Acquisto non riuscito\n");
			else if (*risAcquisto == 1)
				//Tutto ok
				printf("Acquisto riuscito con successo\n");
		} // if 2
		else
			printf("[ERROR]Operazione richiesta non disponibile!!\n");

		printf("Inserire: i per inserire un evento \nInserisci: a per acquistare dei biglietti.\n");

	} // while

	// Libero le risorse, distruggendo il gestore di trasporto
	clnt_destroy(cl);
	free(input);
	free(e);
	printf("TERMINO ");
	exit(0);
} //main
