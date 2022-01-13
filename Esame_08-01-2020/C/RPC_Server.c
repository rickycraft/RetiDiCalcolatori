#include <rpc/rpc.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "RPC_xFile.h"

#define DIM_STRING 16

//
Output *lista_file_carattere_1_svc(Input *dato, struct svc_req *cl)
{
	static Output result;
	
	int i;
	int indice = 0;
	DIR *d;
	struct dirent *dir;

	//printf("quindi ricevo dal client:\n ID:%s\nTIPO:%s\nLUOGO:%s\nDATA: %s\nBigliettiDisponibili: %i\nPrezzo: %i", dato->id, dato->tipo, dato->luogo, dato->data, dato->bigliettiDisponibili, dato->prezzo);

	//printf("dopo if\n");

	//getcwd(precedenteDir, sizeof(precedenteDir));
	char *directory = malloc(strlen(dato->nomeDir) + 1);
	strcpy(directory, dato->nomeDir);
	printf("dir:%s\n", directory);
	strcat(directory, "/");
	d = opendir(directory);
	if (d == NULL)
	{
		printf("D NULLO!\n");
		exit(0);
	}
	char *nomeFile;
	int lunghezzaNome;
	char *tmp;
	char *estensioneFile;
	int fileRimanente;
	int occorrenzaPerFile = 0;

	for(int i=0;i<N;i++){
		result.listaNomeFile[i].nomeFile=malloc(strlen("L")+1);
		strcpy(result.listaNomeFile[i].nomeFile,"L");
		
	}
	result.esito=0;
	printf("pre while\n");
	while ((dir = readdir(d)) != NULL)
	{

		if (dir->d_type == DT_REG)
		{
			//printf("Un file trovato\n");
			lunghezzaNome = strlen(dir->d_name);
			tmp = malloc(lunghezzaNome + 1);
			strcpy(tmp, dir->d_name);
			nomeFile = strtok(tmp, ".");
			printf("nomeFile:%s\n", nomeFile);
			estensioneFile = strtok(NULL, ".");

			if (estensioneFile != NULL)
			{
				printf("estensioneFile:%s e %ld\n\n",estensioneFile,strlen(estensioneFile));
				if (strcmp(estensioneFile, "txt") == 0)
				{
					//quindi è un file di testo
					occorrenzaPerFile = 0;
					for (int i = 0; i < strlen(nomeFile); i++)
					{
						if (nomeFile[i] == dato->carattere)
						{
							occorrenzaPerFile++;
							printf("occorrenza Trovata\n");
						}
					}
					printf("post for: occorrenze -> %d",occorrenzaPerFile);
					if (occorrenzaPerFile >= dato->numOccorrenze && indice < N)
					{
						free(result.listaNomeFile[indice].nomeFile);
						result.listaNomeFile[indice].nomeFile = malloc(strlen(nomeFile) + 1);
						strcpy(result.listaNomeFile[indice].nomeFile, nomeFile);
						indice++;
						result.esito=1;
						printf("aggiunto qualcosa a RESULT\n");
					}
				}
			}
		}
	}
	printf("Post while\n");
	result.indice = indice;
	if(indice==0){
		printf("INDICE NULLO\n");
	}
	printf("Result\n");
	for (int i = 0; i < indice; i++)
	{
		printf("nome:%s\n", result.listaNomeFile[i].nomeFile);
	}

	free(tmp);
	return &result;
}

int *elimina_occorrenze_1_svc(char **stringa, struct svc_req *cl)
{
	int i;
	int fd;
	int n;
	int fw;
	char fileTemp[FILENAME_MAX];
	static int occorrenze = 0;
	char c;
	if (stringa == NULL)
	{
		return NULL;
	}
	fd = open(*stringa, O_RDONLY);
	if (fd == NULL)
	{
		printf("inviato nome non trovato");
		return NULL;
	}
	strcpy(fileTemp, *stringa);
	strcat(fileTemp, "_temp");
	fw = open(fileTemp, O_CREAT | O_WRONLY);
	occorrenze = 0;
	while ((n = read(fd, &c, 1)) > 0)
	{
		if (c >= '0' && c <= '9')
		{
			occorrenze++;
		}
		else
		{
			write(fw, &c, 1);
		}
	}

	rename(fileTemp, *stringa);
	close(fw);
	close(fd);

	return &occorrenze;
}