struct Wc {
  int righe;
  int caratteri;
  int parole;
};

struct NameSize {
  string nome<20>;
  int dim;
};

program OPERAZIONIPROG {
  version OPERAZIONIVERS {
    Wc file_scan(string) = 1;
    int dir_scan(NameSize) = 2;
  } = 1;
} = 0x20000013;