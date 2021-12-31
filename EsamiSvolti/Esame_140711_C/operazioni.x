
const LUNGHEZZA_NOME=30;
const LUNGHEZZA_NOME_FILE=30;
const LUNGHEZZA_PAROLA=10;
const NUM_PAROLE=2;

struct RigaRicerca{
  char nome[LUNGHEZZA_NOME];
  int durata;
  Parola p[NUM_PAROLE];  
  
};

struct Tabella{
  RigaRicerca candidato[8];
};

struct Candidato {
  char nome[LUNGHEZZA_NOME];
  char nomeFile[LUNGHEZZA_NOME_FILE];
  int durata;
  Parola p[NUM_PAROLE];  
  };

struct Parola{
    char parola[LUNGHEZZA_PAROLA];
};

struct Periodo{
  char dataIniziale[8];
  char dataFinale[8];
};

struct CandidatoParola{
  char nomeCandidato[LUNGHEZZA_NOME];
  Parola p;
};

program PROVINIPROG {
  version PROVINIVERS {
    Tabella  ricerca_provini (Periodo) = 1;
    int aggiungi_parola (CandidatoParola) = 2;
  } = 1;
} = 0x20000013;