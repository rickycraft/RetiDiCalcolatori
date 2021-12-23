#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX_STRING_LENGTH 256

void filtra(char *filtro, int fd)
{
  char read_char;
  while (read(fd, &read_char, sizeof(char)) > 0)
  {
    if (strchr(filtro, read_char) == NULL)
      printf("%c", read_char);
  }
}

// consumatore.c e' un filtro
int main(int argc, char *argv[])
{
  char buf[MAX_STRING_LENGTH];
  char *filtro;
  int fd;
  //controllo numero argomenti
  if (argc < 2)
  {
    perror(" numero di argomenti sbagliato");
    exit(1);
  }

  filtro = argv[1];

  if (argc == 3)
  {
    fd = open(argv[2], O_RDONLY);
    if (fd < 0)
    {
      perror("P0: Impossibile aprire il file.");
      exit(2);
    }
    filtra(filtro, fd);
    close(fd);
  }
  else
  {
    filtra(filtro, 0);
  }
}
