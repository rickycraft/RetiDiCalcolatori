#include <rpc/rpc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "RPC_xFile.h"
#define MAX_DATI 10
#define DIM_STRING 16

/*STATO INTERNO PRIVATO DEL SERVER*/
typedef struct{
	char str;
	char ch ;
	int num;
} Dato;

//variabili globali statiche
static Dato dati[MAX_DATI];
static int inizializzato = 0;

/*Stato interno parzialmente implementato*/
void inizializza(){
	int i;

	if (inizializzato == 1) return;

	for (i = 0; i < MAX_DATI; i++){
		strcpy(dati[i].str,"L");
		dati[i].ch='L';
		dari[i].num=-1;
	}
    //alcuni dati da inizializzare
	
		strcpy(dati[1].str,"L");
		dati[1].ch='L';
		dari[1].num=-1;



	//ecc

	inizializzato = 1;
	printf("[DEBUG] Terminata inizializzazione struttura dati!\n");
}

//
int * inserimento_1_svc(Dato *dato, struct svc_req * cl)
{
	inizializza();
	static int esito;
    int i;
    esito=-1;
	if(dato == NULL){
		return &esito;
	}
    for (i = 0; i < MAX_DATI; i++){
        if(strcmp(dati[i].str,(*dato).str )== 0)
			return &esito;
    }

	for (i = 0; i < MAX_DATI; i++){
        if(strcmp(dati[i].str,"L" )== 0){
			strcpy(dati[i].str,(*dato).str);

			//ecc
			esito=0;
			break;
		}
    }
    
	return &esito;

}
int * trova_1_svc(Input* input, struct svc_req *cl){
    inizializza();
	static int esito;
    int i;
    esito=-1;
	if(input == NULL){
		return &esito;
	}
    for (i = 0; i < MAX_DATI; i++)
    {
        if(strcmp(dati[i].str,(*input).str )== 0 && dati[i].num >= *input.num){
			//

			esito=0;
			break;
		}
    }
    
	return &esito;
}