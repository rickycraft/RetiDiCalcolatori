const MAX_STRING=256;
const N=6;


struct Input {
		string nomeDir<MAX_STRING>;
		char carattere;
		int numOccorrenze;
	};


struct NomeFile{
string nomeFile<MAX_STRING>;
};

struct Output{
	NomeFile listaNomeFile[N];
	int indice;
	int esito;
};


program RPCESAME {
	version RPCESAMEVERS {  
		Output lista_file_carattere(Input) = 1;
        int elimina_occorrenze(string)=2;
	} = 1;
} = 0x20000020;