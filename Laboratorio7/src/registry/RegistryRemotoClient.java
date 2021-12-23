package registry;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface RegistryRemotoClient extends Remote{
	
	
	//Server RMI e implementa diverse operazioni
	/*Per i client: 
	 * primo riferimento di un server remoto registrato con nome logico
	 * tutti i riferimenti di un server remoto registrato con nome logico
	 * ricerca per tag dei nomi logici dei servizi cercati*/
	
	
		public Remote cerca(String nomeLogico)throws RemoteException;

		public Remote[] cercaTutti(String nomeLogico)throws RemoteException;

}
