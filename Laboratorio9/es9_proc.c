#include <dirent.h>
#include <fcntl.h>
#include <rpc/rpc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "operazioni.h"
#define GRANDEZZATABELLA 10
#define GRANDEZZAGIUDICI 20

Candidato tabella[GRANDEZZATABELLA];

void creazioneTabella(){
//creo la tabella e la riempio di righe vuote
for(int i=0; i<GRANDEZZATABELLA; i++){

  tabella[i].nomeCandidato="L";
  tabella[i].nomeGiudice="L";
  tabella[i].categoria='L';
  tabella[i].nomeFile="L";
  tabella[i].fase='L';
  tabella[i].voto=-1;
  }
}

int compare(const void * left, const void * right) {
    const Giudici * a = (const Giudici*) left;
    const Giudici* b = (const Giudici *) right;
    if (a->voto > b->voto) {
        return 1;
    } else if (a->voto < b->voto) {
        return -1;
    } else {
        return 0;
    }
}

void aggiungiCandidato(Candidato c, int i){

  tabella[i]=c;
}

/*
restituisce una struttura dati che la classifica dei giudici in ordine di voti
*/
Giudici *classifica_giudici_1_svc(void *in,struct svc_req *rp) {

  if(tabella==NULL){
    creazioneTabella();
    Candidato c={"candidato1","giudice1",'A',"file",'A',12};
    
    aggiungiCandidato(c,0);
   c.nomeCandidato="candidato2";
   c.nomeGiudice="giudice2";
   c.categoria='A';
   c.nomeFile="file";
   c.fase='A';
   c.voto=2;
  aggiungiCandidato(c,1);

  }

  static Giudici ret[GRANDEZZAGIUDICI];
  int celleRiempite=0;
  for(int i=0; i<GRANDEZZATABELLA;i++){
    if(strcmp(tabella[i].nomeGiudice,"L")!=0){
      //Esiste giudice in quella riga della tabella
      int esiste=0;
      for(int j=0;j<GRANDEZZAGIUDICI;j++){
        if(strcmp(tabella[i].nomeGiudice,ret[j].giudice)==0){
          //se è gia presente nella lista dei giudici allora sommo il voto
          ret[j].voto+=tabella[i].voto;
          esiste=1;
        }
      }
      if(!esiste){
        //aggiunto alla lista dei giudici
        ret[celleRiempite].giudice=tabella[i].nomeGiudice;
        ret[celleRiempite].voto=tabella[i].voto;
        celleRiempite++;
      }  
    }
  }
  //finito controllo con tabella
  
  //ora bisogna ordinare
  qsort(ret, sizeof(ret)/sizeof(Giudici), sizeof(Giudici), compare);
  return ret;
}

/*
parametro d’ingresso il nome del candidato e se si vuole aggiungere ridurre il voto
*/


int *esprimi_voto_1_svc(CandidatoVoto *candidato, struct svc_req *rp) {
  if(tabella==NULL){
    creazioneTabella();
  }
  //return 1 se è andato tutto bene, 0 se non ha trovato il candidato
  static int n=0;

  for(int i=0;i<GRANDEZZATABELLA;i++){
    if(strcmp(tabella[i].nomeCandidato,candidato->nome)==0){
      //abbiamo trovato il candidato giusto
      if((candidato->tipoOperazione)=='+'){
        tabella[i].voto++;
      n=1;
      }
      if((candidato->tipoOperazione)=='-'){
         tabella[i].voto--;
      n= 1;
      }
      
    }
  }
  return &n;

}