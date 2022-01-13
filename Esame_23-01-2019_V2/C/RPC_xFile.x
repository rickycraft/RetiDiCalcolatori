const TARGA_LENGTH =10;
const PATENTE_LENGTH =10;
const DATA_LENGTH=11;
const MAX_STRING=256;
const MAX_PRENOTAZIONI=10;

struct Prenotazione {
		string targa<TARGA_LENGTH>;
		string patente<PATENTE_LENGTH>;
		string tipo<DATA_LENGTH>;
		string  dir<MAX_STRING>;
	};


struct Input{
    char targa[TARGA_LENGTH];
    char patente[PATENTE_LENGTH];
};

struct Output{
	Prenotazione prenotazioni[MAX_PRENOTAZIONI];
};


program RPCESAME {
	version RPCESAMEVERS {  
		Output visualizza_prenotazioni(string) = 1;
        int aggiorna_licenza(Input)=2;
	} = 1;
} = 0x20000020;