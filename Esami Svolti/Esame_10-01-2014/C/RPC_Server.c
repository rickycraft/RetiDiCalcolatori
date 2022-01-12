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
static Evento dati[MAX_DATI];
static int inizializzato = 0;

/*Stato interno parzialmente implementato*/
void inizializza()
{
	int i;

	if (inizializzato == 1)
		return;

	for (i = 0; i < MAX_DATI; i++)
	{
		dati[i].id = malloc(strlen("L") + 1);
		dati[i].tipo = malloc(strlen("L") + 1);
		dati[i].data = malloc(strlen("L") + 1);
		dati[i].luogo = malloc(strlen("L") + 1);

		strcpy(dati[i].id, "L");
		strcpy(dati[i].tipo, "L");
		strcpy(dati[i].data, "L");
		strcpy(dati[i].luogo, "L");
		dati[i].prezzo = 0;
		dati[i].bigliettiDisponibili = 0;
	}

	inizializzato = 1;
	printf("[DEBUG] Terminata inizializzazione struttura dati!\n");
}

//
int *inserimento_evento_1_svc(Evento *dato, struct svc_req *cl)
{
	inizializza();
	static int esito;
	int i;
	esito = -1;
	//printf("dentro inserimentoEvento");
	//printf("quindi ricevo dal client:\n ID:%s\nTIPO:%s\nLUOGO:%s\nDATA: %s\nBigliettiDisponibili: %i\nPrezzo: %i", dato->id, dato->tipo, dato->luogo, dato->data, dato->bigliettiDisponibili, dato->prezzo);
	if (dato == NULL)
	{
		return &esito;
	}
	//printf("dopo if\n");
	for (i = 0; i < MAX_DATI; i++)
	{
		if (strcmp(dati[i].id, (*dato).id) == 0)
			return &esito;
	}
	//printf("dopo primo For\n");
	for (i = 0; i < MAX_DATI; i++)
	{
		if (strcmp(dati[i].id, "L") == 0)
		{
			//		printf("dovrei essere dentro if del secondo for\n");
			//ecc
			dati[i].id = malloc(strlen(dato->id) + 1);
			dati[i].data = malloc(strlen(dato->data) + 1);
			dati[i].luogo = malloc(strlen(dato->luogo) + 1);
			dati[i].tipo = malloc(strlen(dato->tipo) + 1);

			strcpy(dati[i].id, dato->id);
			strcpy(dati[i].data, dato->data);
			strcpy(dati[i].luogo, dato->luogo);
			strcpy(dati[i].tipo, dato->tipo);
			dati[i].prezzo = dato->prezzo;
			dati[i].bigliettiDisponibili = dato->bigliettiDisponibili;

			esito = 1;
			break;
		}
	}
	//printf("dopo secondo for. dovrebbe aver dato esito\n");
	return &esito;
}

int *acquista_biglietti_1_svc(Input *input, struct svc_req *cl)
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
		if (strcmp(dati[i].id, (*input).id) == 0 && dati[i].bigliettiDisponibili >= (*input).numBigliettiDaAcquistare)
		{
			//
			dati[i].bigliettiDisponibili -= (*input).numBigliettiDaAcquistare;
			esito = 1;
			break;
		}
	}

	return &esito;
}