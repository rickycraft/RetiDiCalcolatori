#include <string.h>
//PER FORMATTARE C ALT+SHIFT+F
/*PRENDERE SOTTOSTRINGA DI UNA STRINGA*/
char anno[5];   //del concorrente
char mese[3];   //del concorrente
char giorno[3]; //del concorrente
char *data="CONTENUTO";
strncpy(giorno, data, 2);
giorno[2] = '\0'; //Ricordarsi lo \0 altrimenti non termina la stringa
strncpy(mese, data + 2, 2);
mese[2] = '\0';

strncpy(anno, data + 4, 4);
anno[4] = '\0';





/*SPLITTARE IN SUBSTRINGHE CON DELIMITER*/
//in pratica strtok prende una stringa, un delimiter e restituisce la prima stringa. ATTENZIONE viene modificata la stringa da separare con la prima subString
//Quindi meglio utilizzare una copia della vera stringa da splittare pe evitare di perdere dati.
char *stringaDaSeparare = "String Da Separare";
char *primaSubString = strtok(stringaDaSeparare, " ");
// printf("giornoIniziale:%s\n", giornoIniziale);
char *secondaSubString = strtok(NULL, "/");
//  printf("meseIniziale:%s\n", meseIniziale);
char *terzaSubString = strtok(NULL, "/");
// printf("annoIniziale:%s\n", annoIniziale);





/*STAMPA INDIRIZZO DI MEMORIA*/
//stampa res in questo caso ma cambiare il nome per variabile richiesta
printf("indirizzo res:%p\n", (void *)&res);
