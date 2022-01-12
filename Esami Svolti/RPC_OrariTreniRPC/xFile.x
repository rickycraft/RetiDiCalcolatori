const K = 6;
const MAX_STRING =128;

struct Ritardo{
	string id<MAX_STRING>;
	int rit;
};
struct Corsa{
	string id<MAX_STRING>;
	string par<MAX_STRING>;
	string arr<MAX_STRING>;
	int hh;
	int mm;
	int rit;
	string aud<MAX_STRING>;
};
struct Corse{
	Corsa list[K];
	int numero;
};
program OPERATION {
	version OPERATIONVERS {  
		Corse VISUALIZZA(string) = 1;
        int MODIFICA(Ritardo)=2;
	} = 1;
} = 0x20000020;