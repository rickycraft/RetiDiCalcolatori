package server;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface RegistryRemotoServer extends RegistryRemotoClient{
	//Estende RegistryRemotoClient perchè deve includere tutti i metodi che hanno i clientw

	/*Per i server:
	 * TUTTI I SERVIZI DEL CLIENT+
	 * restituire la lista con tutte le coppie nomelogico-riferimento
	 * aggiungere un server remoto dato il nome logico e riferimento remoto
	 * eliminare prima entry del nome logico
	 * eliminare tutte le entry del nome logico
	 * associazione di un tag ad un nome logico già registrato che farà il RegistryRemotoTagServer
	 */
	public Object[][] restituisciTutti() throws RemoteException;
	public boolean aggiungiServer(String nome_logico, Remote rif_remoto) throws RemoteException;
	public boolean eliminaPrimo (String nome_logico) throws RemoteException;
	public boolean eliminaTutti (String nome_logico) throws RemoteException;
	
	
}
