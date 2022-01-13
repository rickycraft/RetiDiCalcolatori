import java.rmi.Remote;
import java.rmi.RemoteException;

public interface IServer extends Remote {

  Oggetto oggetto() throws RemoteException;

  String setProp(String newProp) throws RemoteException;

}
