#include <rpc/rpc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "RPC_xFile.h"
#define MAX_DATI 10
#define DIM_STRING 16

/*STATO INTERNO PRIVATO DEL SERVER*/
/*typedef struct{
	char str;
	char ch ;
	int num;
} Dato;*/

//variabili globali statiche
static Prenotazione dati[MAX_DATI];
static int inizializzato = 0;

/*Stato interno parzialmente implementato*/
void inizializza()
{
	int i;

	if (inizializzato == 1)
		return;

	for (i = 0; i < MAX_DATI; i++)
	{
		dati[i].targa = (char *)malloc(100);
		dati[i].patente = (char *)malloc(100);
		dati[i].tipo = (char *)malloc(100);
		dati[i].dir = (char *)malloc(100);

		strcpy(dati[i].targa, "L");
		strcpy(dati[i].patente, "L");
		strcpy(dati[i].tipo, "L");
		strcpy(dati[i].dir, "L");
	}
	printf("pre add\n");

	strcpy(dati[0].targa, "EF123EF");
	strcpy(dati[0].patente, "12345");
	strcpy(dati[0].tipo, "auto");
	strcpy(dati[0].dir, "/EF1234_IMG");

	strcpy(dati[1].targa, "AC123AC");
	strcpy(dati[1].patente, "12345");
	strcpy(dati[1].tipo, "camper");
	strcpy(dati[1].dir, "/EF1234_IMG");

	strcpy(dati[2].targa, "EX123AC");
	strcpy(dati[2].patente, "12345");
	strcpy(dati[2].tipo, "camper");
	strcpy(dati[2].dir, "/EF1234_IMG");

	inizializzato = 1;
	printf("[DEBUG] Terminata inizializzazione struttura dati!\n");
}

//
Output *visualizza_prenotazioni_1_svc(char **dato, struct svc_req *cl)
{
	inizializza();
	static Output result;
	int i;
	int indice = 0;

	for (i = 0; i < MAX_PRENOTAZIONI; i++)
	{
		result.prenotazioni[i].targa = (char *)malloc(100);
		result.prenotazioni[i].patente = (char *)malloc(100);
		result.prenotazioni[i].tipo = (char *)malloc(100);
		result.prenotazioni[i].dir = (char *)malloc(100);

		strcpy(result.prenotazioni[i].targa, "L");
		strcpy(result.prenotazioni[i].patente, "L");
		strcpy(result.prenotazioni[i].tipo, "L");
		strcpy(result.prenotazioni[i].dir, "L");
	}
	printf("Inizializzato result\n");
	//printf("quindi ricevo dal client:\n ID:%s\nTIPO:%s\nLUOGO:%s\nDATA: %s\nBigliettiDisponibili: %i\nPrezzo: %i", dato->id, dato->tipo, dato->luogo, dato->data, dato->bigliettiDisponibili, dato->prezzo);

	//printf("dopo if\n");
	for (i = 0; i < MAX_DATI; i++)
	{
		if (strcmp(dati[i].tipo, *dato) == 0)
		{
			printf("trovata occorrenza\n");
			char subTarga[3];
			strncpy(subTarga, dati[i].targa, 2);
			subTarga[2] = '\0';
			if (strcmp(subTarga, "ED") > 0)
			{

				strcpy(result.prenotazioni[indice].targa, dati[i].targa);
				strcpy(result.prenotazioni[indice].patente, dati[i].patente);
				strcpy(result.prenotazioni[indice].tipo, dati[i].tipo);
				strcpy(result.prenotazioni[indice].dir, dati[i].dir);
				indice++;
			}
		}
	}

	printf("Result\n");
	for (int i = 0; i < MAX_PRENOTAZIONI; i++)
	{

		if (strcmp(result.prenotazioni[i].targa, "L") != 0)
		{
			printf("targa :%s \tPatente:%s\tTipo:%s\n", (result.prenotazioni[i].targa), result.prenotazioni[i].patente, result.prenotazioni[i].tipo);
		}
	}

	return &result;
}

int *aggiorna_licenza_1_svc(Input *input, struct svc_req *cl)
{
	inizializza();
	static int esito;
	int i;
	esito = -1;
	if (input == NULL)
	{
		return &esito;
	}
	for (i = 0; i < MAX_DATI; i++)
	{
		if (strcmp(dati[i].targa, input->targa) == 0)
		{
			//
			strcpy(dati[i].patente, input->patente);
			esito = 1;
			break;
		}
	}

	return &esito;
}