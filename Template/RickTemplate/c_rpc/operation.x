const MAX_STRING = 16;

struct Dato {
		string contenuto<MAX_STRING>;
};

program OPERATION {
	version OPERATIONVERS {  
		Dato GET() = 1;
    int SET(Dato) = 2;
	} = 1;
} = 0x20000020;