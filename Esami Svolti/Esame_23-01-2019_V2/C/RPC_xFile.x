const TARGA_LENGTH =8;
const PATENTE_LENGTH =5;
const DATA_LENGTH=11;
const MAX_STRING=256;
const MAX_PRENOTAZIONI=10;

struct Prenotazione {
		char targa[TARGA_LENGTH];
		char patente[PATENTE_LENGTH];
		char tipo[DATA_LENGTH];
		char dir[MAX_STRING];
	};


struct Input{
    char targa[TARGA_LENGTH];
    char patente[PATENTE_LENGTH];
};

struct Output{
	Prenotazione prenotazioni[MAX_PRENOTAZIONI];
};

struct Tipo{
	char tipo[DATA_LENGTH];
};

program RPCESAME {
	version RPCESAMEVERS {  
		Output visualizza_prenotazioni(Tipo) = 1;
        int aggiorna_licenza(Input)=2;
	} = 1;
} = 0x20000020;