#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct {
  int righe;
  int caratteri;
  int parole;
} wc;

/*
parametro d’ingresso il nome del file e restituisce una struttura dati
che contiene tre interi corrispondenti al numero di caratteri
parole e linee nel file, oppure un opportuno codice di errore in
caso ad esempio il file sia vuoto oppure non sia presente sul server
*/
wc *file_scan(char *nome_file) {
  FILE *fp;
  char righe[10], parole[10], caratteri[10];
  char command[1024] = "/usr/bin/wc ";
  static wc ret;

  fp = popen(strcat(command, nome_file), "r");
  if (fp == NULL) return NULL;

  if (fscanf(fp, "%s %s %s", righe, parole, caratteri) == 3) {
    ret.righe = atoi(righe);
    ret.parole = atoi(parole);
    ret.caratteri = atoi(caratteri);
  }
  pclose(fp);
  return &ret;
}

/*
parametro d’ingresso il nome del direttorio remoto e una soglia numerica In
caso di successo, restituisce un intero positivo con il
numero di file la cui dimensione supera la soglia inserita, -1 altrimenti
*/
long int stat_filesize(char *filename) {
  struct stat statbuf;
  if (stat(filename, &statbuf) == -1) {
    printf("failed to stat %s\n", filename);
    return -1;
  }
  return statbuf.st_size;
}

int dir_scan(char *dirname, int soglia) {
  struct dirent *dd;
  DIR *dir;
  const char *current_dir = ".";
  int n = 0;
  if (chdir(dirname) < 0) return -1;
  dir = opendir(current_dir);
  while ((dd = readdir(dir)) != NULL) {
    if (dd->d_type == DT_REG)
      if (stat_filesize(dd->d_name) > soglia) n++;
  }
  closedir(dir);
  return n;
}

int main(int argc, char *argv[]) {
  // printf("stat %d\n", dir_scan(argv[1], 10000));
  wc *wcs = file_scan(argv[1]);
  printf("righe %d, parole %d, caratteri %d\n", wcs->righe, wcs->parole,
         wcs->caratteri);
}