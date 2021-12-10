struct Giudici {
  string giudice<30>;
  int voto;
  };

struct Candidato{

  string nomeCandidato<30>;
  string nomeGiudice<30>;
  char categoria;
  string nomeFile<30>;
  char fase;
  int voto;
};

struct CandidatoVoto {
  string nome<30>;
  char tipoOperazione;
};

program OPERAZIONIPROG {
  version OPERAZIONIVERS {
    Giudici classifica_giudici(void) = 1;
    int esprimi_voto(CandidatoVoto) = 2;
  } = 1;
} = 0x20000013;