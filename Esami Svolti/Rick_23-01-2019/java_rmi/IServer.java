import java.rmi.Remote;
import java.rmi.RemoteException;
import java.util.List;

public interface IServer extends Remote {

  int elimina_prenotazione(String targa) throws RemoteException;

  List<Prenotazione> visualizza_prenotazioni() throws RemoteException;

}
