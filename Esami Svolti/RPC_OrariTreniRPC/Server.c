#include <rpc/rpc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xFile.h"
#define MAX_DATI 10
#define MAX_STRING 128
typedef struct {
    char *id;
    char *tipo;
    char *partenza;
    char *arrivo;
    char *audio;
    int hh;
    int mm;
    int ritardo;

} Dato;
/*STATO INTERNO PRIVATO DEL SERVER*/
// variabili globali statiche
static Dato dati[MAX_DATI];
static int inizializzato = 0;

/*Stato interno parzialmente implementato*/
void inizializza() {
    int i;
    printf("sto inizializzando\n");
    if (inizializzato == 1) return;

    for (i = 0; i < MAX_DATI; i++) {
        dati[i].id = (char *)malloc(strlen("L") + 1);
        strcpy(dati[i].id, "L");
        dati[i].tipo = (char *)malloc(strlen("L") + 1);
        strcpy(dati[i].tipo, "L");
        dati[i].partenza = (char *)malloc(strlen("L") + 1);
        strcpy(dati[i].partenza, "L");
        dati[i].arrivo = (char *)malloc(strlen("L") + 1);
        strcpy(dati[i].arrivo, "L");
        dati[i].audio = (char *)malloc(strlen("L") + 1);
        strcpy(dati[i].audio, "L");
        dati[i].hh = -1;
        dati[i].mm = -1;
        dati[i].ritardo = -1;
    }
    // alcuni dati da inizializzare
    free(dati[0].id);
    dati[0].id = (char *)malloc(strlen("SATA1234") + 1);
    strcpy(dati[0].id, "SATA1234");
    free(dati[0].tipo);
    dati[0].tipo = (char *)malloc(strlen("Partenza") + 1);
    strcpy(dati[0].tipo, "Partenza");
    free(dati[0].partenza);
    dati[0].partenza = (char *)malloc(strlen("Bologna") + 1);
    strcpy(dati[0].partenza, "Bologna");
    free(dati[0].arrivo);
    dati[0].arrivo = (char *)malloc(strlen("Bari") + 1);
    strcpy(dati[0].arrivo, "Bari");
    free(dati[0].audio);
    dati[0].audio = (char *)malloc(strlen("SATA1234.mp4") + 1);
    strcpy(dati[0].audio, "SATA1234.mp4");
    dati[0].hh = 12;
    dati[0].mm = 15;
    dati[0].ritardo = 0;

    free(dati[1].id);
    dati[1].id = (char *)malloc(strlen("MATA1234") + 1);
    strcpy(dati[1].id, "MATA1234");
    free(dati[1].tipo);
    dati[1].tipo = (char *)malloc(strlen("Arrivo") + 1);
    strcpy(dati[1].tipo, "Arrivo");
    free(dati[1].partenza);
    dati[1].partenza = (char *)malloc(strlen("Milano") + 1);
    strcpy(dati[1].partenza, "Milano");
    free(dati[1].arrivo);
    dati[1].arrivo = (char *)malloc(strlen("Bologna") + 1);
    strcpy(dati[1].arrivo, "Bologna");
    free(dati[1].audio);
    dati[1].audio = (char *)malloc(strlen("MATA1333.mp4") + 1);
    strcpy(dati[1].audio, "MATA1333.mp4");
    dati[1].hh = 16;
    dati[1].mm = 30;
    dati[1].ritardo = 21;

    free(dati[2].id);
    dati[2].id = (char *)malloc(strlen("KATA1234") + 1);
    strcpy(dati[2].id, "KATA1234");
    free(dati[2].tipo);
    dati[2].tipo = (char *)malloc(strlen("Arrivo") + 1);
    strcpy(dati[2].tipo, "Arrivo");
    free(dati[2].partenza);
    dati[2].partenza = (char *)malloc(strlen("Culo") + 1);
    strcpy(dati[2].partenza, "Culo");
    free(dati[2].arrivo);
    dati[2].arrivo = (char *)malloc(strlen("Tette") + 1);
    strcpy(dati[2].arrivo, "Tette");
    free(dati[2].audio);
    dati[2].audio = (char *)malloc(strlen("Culo.mp4") + 1);
    strcpy(dati[2].audio, "Culo.mp4");
    dati[2].hh = 16;
    dati[2].mm = 30;
    dati[2].ritardo = 21;

    // ecc

    inizializzato = 1;
    stampa();
    printf("[DEBUG] Terminata inizializzazione struttura dati!\n");
}
void stampa()  // metodo di stampa usato per il DEBUG interno
{
    printf("stampa di debug\n");
    for (int i = 0; i < MAX_DATI; i++) {
        printf("%s \t", dati[i].id);
        printf("%s \t", dati[i].tipo);
        printf("%s \t", dati[i].partenza);
        printf("%s \t", dati[i].arrivo);
        printf("%d:%d \t", dati[i].hh, dati[i].mm);
        printf("%d \t", dati[i].ritardo);
        printf("%s \n", dati[i].audio);
    }
}

// PROCEDURA 1
Corse *visualizza_1_svc(char **tipo, struct svc_req *cl) {
    printf("[DEBUG] visualizza tipo %s!\n", *tipo);
    inizializza();

    static Corse ris;
    int i;
    ris.numero = -1;
    if (*tipo == NULL) {
        return &ris;
    }
    for (int i = 0; i < MAX_DATI; i++) {
        /* code */
        ris.list[i].id = malloc(strlen("L") + 1);
        strcpy(ris.list[i].id, "L");
        ris.list[i].par = malloc(strlen("L") + 1);
        strcpy(ris.list[i].par, "L");
        ris.list[i].arr = malloc(strlen("L") + 1);
        strcpy(ris.list[i].arr, "L");
        ris.list[i].aud = malloc(strlen("L") + 1);
        strcpy(ris.list[i].aud, "L");
        ris.list[i].hh = -1;
        ris.list[i].mm = -1;
        ris.list[i].rit = -1;
    }
    //
    ris.numero = 0;
    printf("cerco la lista\n");
    for (i = 0; i < MAX_DATI; i++) {
        if (strcmp(dati[i].tipo, *tipo) == 0) {
            printf("[DEBUG] trovato tipo con id %s\n", dati[i].id);

            ris.list[ris.numero].id = malloc(strlen(dati[i].id) + 1);
            strcpy(ris.list[ris.numero].id, dati[i].id);
            ris.list[ris.numero].par = malloc(strlen(dati[i].partenza) + 1);
            strcpy(ris.list[ris.numero].par, dati[i].partenza);
            ris.list[ris.numero].arr = malloc(strlen(dati[i].arrivo) + 1);
            strcpy(ris.list[ris.numero].arr, dati[i].arrivo);
            ris.list[ris.numero].aud = malloc(strlen(dati[i].audio) + 1);
            strcpy(ris.list[ris.numero].aud, dati[i].audio);
            ris.list[ris.numero].hh = dati[i].hh;
            ris.list[ris.numero].mm = dati[i].mm;
            ris.list[ris.numero].rit = dati[i].ritardo;
            ris.numero++;
        }
    }
    printf("lista:\n");
    for (i = 0; i < ris.numero; i++) {
        /* code */
        printf("%s\n", ris.list[i].id);
    }

    printf("[DEBUG] fine !\n");

    return &ris;
}

// PROCEDURA 2
int *modifica_1_svc(Ritardo *rit, struct svc_req *cl) {
    inizializza();
    static int esito;

    printf("[DEBUG] acquista!\n");
    int i;
    esito = -1;

    if (rit == (NULL)) {
        return &esito;
    }

    for (i = 0; i < MAX_DATI; i++) {
        if (strcmp(dati[i].id, rit->id) == 0) {
            printf("[DEBUG] trovato corsa da aggiornare &s!\n", dati[i].id);
            dati[i].ritardo = rit->rit;
            esito = 1;
            break;
        }
    }
    printf("[DEBUG] fine %d!\n", esito);
    stampa();

    return &esito;
}