const NUMGIUDICI=3;
const LUNGHEZZA_NOME_GIUDICE=30;
const LUNGHEZZA_NOME_CANDIDATO=30;

struct Giudice {
  string nome<256>;
  int votiTotali;
  };

struct Giudici{
    Giudice giudici_ordinati[NUMGIUDICI];
    
};

struct Candidato{

  char *nomeCandidato;
  char *nomeGiudice;
  char categoria;
  char *nomeFile;
  char fase;
  int voto;
};

struct CandidatoVoto {
  string nome<256>;
  char tipoOperazione;
};

program OPERAZIONIPROG {
  version OPERAZIONIVERS {
    Giudici classifica_giudici(void) = 1;
    int esprimi_voto(CandidatoVoto) = 2;
  } = 1;
} = 0x20000013;