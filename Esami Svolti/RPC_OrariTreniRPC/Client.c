
#include <rpc/rpc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xFile.h"

#define MAX_STRING 128  //
int isNumber(char *text) {
    int j;
    j = strlen(text);
    while (j--) {
        if (text[j] >= '0' && text[j] <= '9') continue;

        return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    char *host;               // nome host
    CLIENT *cl;               // gestore del trasporto
    char buffer[MAX_STRING];  // buffer per leggere stringhe
    // dichiarazioni delle variabili
    int i, *ris;
    char c;
    Corse *corse;

    /****************************************************************************************************************/
    // CONTROLLO DEGLI ARGOMENTI
    if (argc != 2) {
        printf("[ERROR] usage: %s server_host\n", argv[0]);
        exit(1);
    }
    host = argv[1];

    // CREAZIONE GESTORE DI TRASPORTO
    cl = clnt_create(host, OPERATION, OPERATIONVERS, "udp");
    if (cl == NULL) {
        clnt_pcreateerror(host);
        exit(1);
    }

    // INTERAZIONE CON L UTENTE

    printf("Inserire:\n1\t visualizza \n 2\t aggiorna \n^D\tper terminare: ");

    while (gets(buffer) != 0) {
        // OPERAZIONE 1

        char *tipo;
        if (strcmp(buffer, "1") == 0) {
            do {
                printf("inserire tipo\n");
                gets(buffer);
                if (strcmp(buffer, "Partenza") == 0 ||
                    strcmp(buffer, "Arrivo") == 0)
                    break;
            } while (1);
            tipo = (char *)malloc(strlen(buffer) + 1);
            strcpy(tipo, buffer);
            printf("operazione evocata con %s", tipo);

            // Invocazione remota
            corse = visualizza_1(&tipo, cl);
            free(tipo);
            // Controllo del risultato se punta ad un indirizzo vuoto
            if ((corse == NULL)) {
                // Errore di RPC
                clnt_perror(cl, host);
                exit(1);
            }

            printf("Operazione fatta \n");
            // il valore all indirizzo puntato
            if ((corse->numero) < 0) {
                printf("Problema del server");
            } else {
                printf("operazione riuscita con esito %d\n", corse->numero);
                for (int i = 0; i < corse->numero; i++) {
                    /* code */
                    printf("%s \t", corse->list[i].id);
                    printf("%s \t", corse->list[i].par);
                    printf("%s \t", corse->list[i].arr);
                    printf("%d:%d \t", corse->list[i].hh, corse->list[i].mm);
                    printf("%d \t", corse->list[i].rit);
                    printf("%s \n", corse->list[i].aud);
                }
            }

        }  // if 1

        // SE OPERAZIONE 2
        else if (strcmp(buffer, "2") == 0) {
            Ritardo ritardo;
            do {
                printf("inserire il id\n");
                gets(buffer);
                if (buffer != NULL) break;
            } while (1);
            ritardo.id = (char *)malloc(strlen(buffer) + 1);
            strcpy(ritardo.id, buffer);

            do {
                printf("inserire il ritardo\n");
                if (gets(buffer) != 0 && isNumber(buffer)) break;
            } while (1);

            ritardo.rit = atoi(buffer);
            printf("operazione evocata con %s %d", ritardo.id, ritardo.rit);
            // Invocazione remota
            ris = modifica_1(&ritardo, cl);
            // Controllo del risultato
            if (ris == NULL) {
                // Errore di RPC
                clnt_perror(cl, host);
                exit(1);
            }

            if (*ris < 0)
                // Eventuale errore di logica del programma
                printf("[ERROR] nel server");
            else if (*ris == 0)
                // Tutto ok
                printf("Success con %d\n", *ris);
        }  // if 2
        // ALTRIMENTI
        else
            printf("[ERROR]Operazione richiesta non disponibile!!\n");

        printf(
            "Inserire:\n1\t visualizza \n 2\t modifica \n^D\tper terminare: ");

    }  // while

    // Libero le risorse, distruggendo il gestore di trasporto
    clnt_destroy(cl);
    printf("TERMINO ");
    exit(0);
}  // main
