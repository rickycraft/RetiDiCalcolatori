import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class Server extends UnicastRemoteObject implements IServer {

  private Oggetto oggetto;

  protected Server() throws RemoteException {
    super();
    oggetto = new Oggetto();
  }

  @Override
  public Oggetto oggetto() throws RemoteException {
    return this.oggetto;
  }

  @Override
  public String setProp(String newProp) throws RemoteException {
    this.oggetto.editProp(newProp);
    return this.oggetto.proprieta;
  }

  public static void main(String[] args) {

    final int REGISTRYPORT = 1099;
    String registryHost = "localhost";
    String serviceName = "Server";

    // Registrazione del servizio RMI
    String completeName = "//" + registryHost + ":" + REGISTRYPORT + "/" + serviceName;
    try {
      Server serverRMI = new Server();
      Naming.rebind(completeName, serverRMI);
      System.out.println("server# Server RMI: Servizio \"" + serviceName + "\" registrato");
    } catch (Exception e) {
      System.err.println("server# Server RMI \"" + serviceName + "\": " + e.getMessage());
      e.printStackTrace();
      System.exit(1);
    }
  }

}
