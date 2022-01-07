const DATI = 10;
const MAX_STRING =16;

struct Dato {
		string str<MAX_STRING>;
		char ch;
        int num;
	};


struct Input{
    string str <MAX_STRING>;
    int num;
}

program OPERATION {
	version OPERATIONVERS {  
		int OPORDINA(Dato) = 1;
        int OPTROVA(Input)=2;
	} = 1;
} = 0x20000020;