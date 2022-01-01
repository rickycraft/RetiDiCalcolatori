#include <string.h>

/*PRENDERE SOTTOSTRINGA DI UNA STRINGA*/


/*SPLITTARE IN SUBSTRINGHE CON DELIMITER*/
    //in pratica strtok prende una stringa, un delimiter e restituisce la prima stringa. ATTENZIONE viene modificata la stringa da separare con la prima subString
    //Quindi meglio utilizzare una copia della vera stringa da splittare pe evitare di perdere dati.
    char *stringaDaSeparare="String Da Separare";
     char *primaSubString = strtok(stringaDaSeparare, " "); 
  // printf("giornoIniziale:%s\n", giornoIniziale);
  char *secondaSubString = strtok(NULL, "/");
  //  printf("meseIniziale:%s\n", meseIniziale);
  char *terzaSubString = strtok(NULL, "/");
  // printf("annoIniziale:%s\n", annoIniziale);


  /*STAMPA INDIRIZZO DI MEMORIA*/
  //stampa res in questo caso ma cambiare il nome per variabile richiesta
printf("indirizzo res:%p\n", (void *)&res); 
