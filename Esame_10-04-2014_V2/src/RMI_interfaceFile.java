import java.rmi.Remote;
import java.rmi.RemoteException;

public interface RMI_interfaceFile extends Remote {

	/*accetta come parametro d’ingresso la struttura dati di
	inserimento contenente la descrizione dell’evento, il tipo, la data, il luogo, la sua disponibilità, il
	suo prezzo, e aggiunge l’evento alla struttura dati; quindi restituisce l’esito dell’operazione, 0 se
	la registrazione è andata a buon fine, -1 altrimenti,*/
	int inserimento_evento(String id,String tipo,String luogo,String data, int nBiglietti, int prezzo) throws RemoteException;
	
	
	
	/*accetta come parametro d’ingresso una struttura che indica
	contenente la stringa che indica la descrizione dell’evento e un intero che indica il numero di
	biglietti da acquistare; quindi il server valuta la richiesta, controlla se esiste l’evento e il numero
	di biglietti richiesti e aggiorna la struttura restituendo l’esito dell’operazione, 0 se l’acquisto è
	andato a buon fine, -1 altrimenti*/
	int acquista_biglietti(String id, int numBiglietti) throws RemoteException;
}
