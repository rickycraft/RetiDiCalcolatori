#include <rpc/rpc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xFile.h"
#define MAX_DATI 10
#define MAX_STRING 128
typedef struct {
    char targa[MAX_STRING];
    char tipo[MAX_STRING];
    char patente[6];
    char immagini[MAX_STRING];

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
        strcpy(dati[i].targa, "L");
        strcpy(dati[i].patente, "0");
        strcpy(dati[i].tipo, "L");
        strcpy(dati[i].immagini, "L");
    }
    // alcuni dati da inizializzare

    strcpy(dati[1].targa, "FE745NL");
    strcpy(dati[1].patente, "50006");
    strcpy(dati[1].tipo, "camper");
    strcpy(dati[1].immagini, "immagine1");

    strcpy(dati[0].targa, "AN745NL");
    strcpy(dati[0].patente, "00003");
    strcpy(dati[0].tipo, "auto");
    strcpy(dati[0].immagini, "immagine0");
    strcpy(dati[2].targa, "ZZ745NL");
    strcpy(dati[2].patente, "99999");
    strcpy(dati[2].tipo, "camper");
    strcpy(dati[2].immagini, "immagine9");
    // ecc

    inizializzato = 1;
    stampa();
    printf("[DEBUG] Terminata inizializzazione struttura dati!\n");
}
void stampa()  // metodo di stampa usato per il DEBUG interno
{
    printf("stampa di debug\n");
    for (int i = 0; i < MAX_DATI; i++) {
        printf("%s \t", dati[i].targa);
        printf("%s \t", dati[i].patente);
        printf("%s \t", dati[i].tipo);
        printf("%s \n", dati[i].immagini);
    }
}

// PROCEDURA 1
Prenotazioni *visualizza_1_svc(char **tipo, struct svc_req *cl) {
    printf("[DEBUG] visualizza tipo %s!\n", *tipo);
    inizializza();

    static Prenotazioni pre;
    int i;
    pre.numero = -1;
    if (*tipo == NULL) {
        return &pre;
    }
    for (int i = 0; i < MAX_DATI; i++) {
        /* code */
        pre.list[i].targa = malloc(strlen("L") + 1);
        strcpy(pre.list[i].targa, "L");
        pre.list[i].patente = malloc(strlen("L") + 1);
        strcpy(pre.list[i].patente, "L");
        pre.list[i].tipo = malloc(strlen("L") + 1);
        strcpy(pre.list[i].tipo, "L");
        pre.list[i].immagini = malloc(strlen("L") + 1);
        strcpy(pre.list[i].immagini, "L");
    }
    //
    pre.numero = 0;
    printf("cerco la lista\n");
    for (i = 0; i < MAX_DATI; i++) {
        if (strncmp(dati[i].targa, "EN", 2) > 0 &&
            strcmp(dati[i].targa, "L") != 0 &&
            strcmp(dati[i].tipo, *tipo) == 0) {
            printf("[DEBUG] trovato targa maggiore! %s\n", dati[i].targa);

            pre.list[pre.numero].targa = malloc(strlen(dati[i].targa) + 1);
            strcpy(pre.list[pre.numero].targa, dati[i].targa);
            pre.list[pre.numero].patente = malloc(strlen(dati[i].patente) + 1);
            strcpy(pre.list[pre.numero].patente, dati[i].patente);
            pre.list[pre.numero].tipo = malloc(strlen(dati[i].tipo) + 1);
            strcpy(pre.list[pre.numero].tipo, dati[i].tipo);
            pre.list[pre.numero].immagini =
                malloc(strlen(dati[i].immagini) + 1);
            strcpy(pre.list[pre.numero].immagini, dati[i].immagini);
            pre.numero++;
        }
    }
    for (i = 0; i < pre.numero; i++) {
        /* code */
        printf("lista:\n");
        printf("%s\n", pre.list[i].patente);
        printf("%s\n", pre.list[i].tipo);
        printf("%s\n", pre.list[i].immagini);
    }

    printf("[DEBUG] fine !\n");

    return &pre;
}

// PROCEDURA 2
int *aggiorna_1_svc(Licenza *lic, struct svc_req *cl) {
    inizializza();
    static int esito;

    printf("[DEBUG] acquista!\n");
    int i;
    esito = -1;

    if (lic == (Licenza *)NULL) {
        return &esito;
    }

    for (i = 0; i < MAX_DATI; i++) {
        if (strcmp(dati[i].targa, lic->targa) == 0) {
            printf("[DEBUG] trovato targa da aggiornare!\n");
            strcpy(dati[i].patente, lic->patente);
            esito = 1;
            break;
        }
    }
    printf("[DEBUG] fine %d!\n", esito);
    stampa();

    return &esito;
}