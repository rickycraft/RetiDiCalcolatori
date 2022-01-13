import java.rmi.Remote;
import java.rmi.RemoteException;


public interface RMI_interfaceFile extends Remote {

	String[] lista_file(String dir) throws RemoteException;
	
	
	int numerazione_righe(String nomeFile) throws RemoteException;
}
