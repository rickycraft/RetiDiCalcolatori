const MAX_STRING =256;
const DATA_LENGTH=11;

struct Evento {
		string id<MAX_STRING>;
		string tipo<MAX_STRING>;
		string data<DATA_LENGTH>;
		string luogo<MAX_STRING>;
		
        int bigliettiDisponibili;
        int prezzo;
	};


struct Input{
    string id <MAX_STRING>;
    int numBigliettiDaAcquistare;
};

program RPCESAME {
	version RPCESAMEVERS {  
		int inserimento_evento(Evento) = 1;
        int acquista_biglietti(Input)=2;
	} = 1;
} = 0x20000020;