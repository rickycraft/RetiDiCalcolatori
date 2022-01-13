const K = 10;
const MAX_STRING =128;

struct Licenza{
	string targa<MAX_STRING>;
	string patente<5>;
};
struct Prenotazione{
	string targa<MAX_STRING>;
	string patente<MAX_STRING>;
	string tipo<MAX_STRING>;
	string immagini<MAX_STRING>;
};
struct Prenotazioni{
	Prenotazione list[K];
	int numero;
};
program OPERATION {
	version OPERATIONVERS {  
		Prenotazioni VISUALIZZA(string) = 1;
        int AGGIORNA(Licenza)=2;
	} = 1;
} = 0x20000020;