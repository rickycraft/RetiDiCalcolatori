package severRighe;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface RemOp extends Remote {

	/*
	 * In pratica Conta righe riceve nome di un file, il numero minimo di parole
	 * presenti in una riga e stampa il numero di righe che hanno un numero di
	 * parole maggiore del numMinimo
	 */

	int conta_righe(String nomeFile, int numMin) throws RemoteException;

	/*
	 * In ingresso ha nomeFile e un intero. se il file Esiste ed ha un numero di
	 * righe MAGGIORE O UGUALE al parametro d'ingresso allora restituisce il nome
	 * del file modificato e il numero di righe del file modificato e uso una classe
	 * FileRighe con all'interno quelle due variabili.
	 * 
	 * 
	 * 
	 * Non viene esplicitato dalla proposta ma immagino che debba eliminare la riga
	 * ennesima che corrisponde al parametro intero che diamo in ingresso
	 */
	String elimina_riga(String nomeFile, int num) throws RemoteException;

}
