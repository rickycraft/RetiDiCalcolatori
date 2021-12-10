#include <dirent.h>
#include <fcntl.h>
#include <rpc/rpc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "operazioni.h"


struct Candidato{

  char nomeCandidato[30];
  char nomeGiudice[30];
  char categoria;
  char nomeFile[30];
  char fase;
  int voto;
};

const int grandezzaTabella=30;


static Candidato tabella[grandezzaTabella];



/*
restituisce una struttura dati che la classifica dei giudici in ordine di voti
*/
Giudici *classifica_giudici(struct svc_req *rp) {
  
  static Giudici ret[20];
  


  fd = open(*filename, O_RDONLY);
  if (fd < 0) {
    ret.caratteri == -1;
    return &ret;
  }

  while (read(fd, &read_char, sizeof(char)) > 0) {
    if (read_char == '\n') {
      ret.righe++;
      ret.parole++;
    } else if (read_char == ' ') {
      ret.parole++;
    }
    ret.caratteri++;
  }

  close(fd);
  return &ret;
}

/*
Wc *file_scan_1_svc(char **dirname, struct svc_req *rp) {
  FILE *fp;
  char righe[10], parole[10], caratteri[10];
  char command[1024] = "/usr/bin/wc ";
  static Wc ret;

  fp = popen(strcat(command, *dirname), "r");
  if (fp == NULL) return NULL;

  if (fscanf(fp, "%s %s %s", righe, parole, caratteri) == 3) {
    ret.righe = atoi(righe);
    ret.parole = atoi(parole);
    ret.caratteri = atoi(caratteri);
  } else {
    ret.righe = -1;
    ret.parole = -1;
    ret.caratteri = -1;
  }
  pclose(fp);
  return &ret;
}
*/

/*
parametro d’ingresso il nome del direttorio remoto e una soglia numerica. In
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

int *dir_scan_1_svc(NameSize *namesize, struct svc_req *rp) {
  struct dirent *dd;
  DIR *dir;
  const char *current_dir = ".";
  char cwd[1024];
  getcwd(cwd, 1024);
  static int n;
  if (chdir(namesize->nome) < 0) {
    n = -1;
    return &n;
  } else {
    n = 0;
  }
  dir = opendir(current_dir);
  while ((dd = readdir(dir)) != NULL) {
    if (dd->d_type == DT_REG)
      if (stat_filesize(dd->d_name) > namesize->dim) n++;
  }
  closedir(dir);
  chdir(cwd);
  return &n;
}