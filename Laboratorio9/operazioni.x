struct Giudici {
  string giudice<30>;
  int voto;
  };

struct CandidatoVoto {
  string nome<30>;
  string tipoOperazione<1>;
};

program OPERAZIONIPROG {
  version OPERAZIONIVERS {
    Giudici classifica_giudici() = 1;
    void esprimi_voto(CandidatoVoto) = 2;
  } = 1;
} = 0x20000013;