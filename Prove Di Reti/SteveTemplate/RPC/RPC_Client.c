
#include <rpc/rpc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "RPC_xFile.h"

#define DIM 16
//

int main (int argc, char *argv[])
{
	char *host; //nome host
  	CLIENT *cl; //gestore del trasporto
	char buffer[DIM];
	//dichiarazioni delle variabili
	int i, *ris, numVoti=-1;
  	char c;
  	Output *arrayOut;
	void *voidValue;
	Input * input;

  /****************************************************************************************************************/

	//CONTROLLO DEGLI ARGOMENTI
  	if (argc != 2){
    		printf ("[ERROR] usage: %s server_host\n", argv[0]);
    		exit (1);
  	}
  	host = argv[1];

	//CREAZIONE GESTORE DI TRASPORTO
	cl = clnt_create (host, OPERATION, OPERATIONVERS, "udp");
	if (cl == NULL){
		clnt_pcreateerror (host);
		exit (1);
	}

	//INTERAZIONE CON L UTENTE
	printf("Inserire:\n1\t Ordinamento \n2\t trova \n^D\tper terminare: ");

	while (scanf("%s",buffer)!=0){

        //se operazione 1
		if( strcmp(buffer,"1")==0 ){
			void *v;
			// Invocazione remota
			arrayOut = OpOrdina_1(v, cl);

			//Controllo del risultato
			if(arrayOut == NULL){
				//Errore di RPC
				clnt_perror(cl, host);
				exit(1);
			}

			printf("Ordinata \n");

			for(int i=0; i<N; i++) {
				if(arrayOut->arrayElemento[i].tot > 0) {
						printf("%s con %d pt \n", arrayOut->arrayElemento[i].nome, arrayOut->arrayElemento[i].tot);
				}
			}

		} // if 1
        //ser operazione 2
		else if( strcmp(buffer,"2")==0 ){
			printf("\nInserisci str: \n");
			scanf("%s", input->str);
			getchar();

			printf("\nInserisci tipo di operazione (A (addiziona) oppure (S) sottrai ): \n");
			scanf("%c", &input->tipoOp);

			// Invocazione remota
			ris = opTrova_1(input, cl);

			//Controllo del risultato
			if(ris == NULL){
				//Errore di RPC
				clnt_perror(cl, host);
				exit(1);
			}

			if(*ris < 0)
				//Eventuale errore di logica del programma
                printf("[ERROR]");
			else if(*ris == 0)
				//Tutto ok
				printf("Success\n");
		} // if 2
		else
			printf("[ERROR]Operazione richiesta non disponibile!!\n");

		printf("Inserire:\n1\tOrdina\n2\ttrova\n^D\tper terminare: ");
	} // while

	// Libero le risorse, distruggendo il gestore di trasporto
	clnt_destroy (cl);
	printf("TERMINO ");
	exit(0);
}//main
