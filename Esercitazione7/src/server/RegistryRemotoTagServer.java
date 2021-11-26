package server;

import java.rmi.RemoteException;

public interface RegistryRemotoTagServer extends RegistryRemotoServer,RegistryRemotoTagClient {
	
	public int associaTag(String nome_logico_server,String tag) throws RemoteException;

}
