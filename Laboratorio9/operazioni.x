const NUMGIUDICI=3;
const LUNGHEZZA_NOME_GIUDICE=30;
const LUNGHEZZA_NOME_CANDIDATO=30;

struct Giudice {
  string nome<256>;
  int votiTotali;
  };

struct Output{
    Giudice giudici_ordinati[NUMGIUDICI];
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