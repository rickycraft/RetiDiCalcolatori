import java.rmi.Remote;
import java.rmi.RemoteException;


public interface RMI_interfaceFile extends Remote {

	int elimina_prenotazione(String targa) throws RemoteException;
	
	
	Prenotazione[] visualizza_prenotazioni(String tipoVeicolo) throws RemoteException;
}
