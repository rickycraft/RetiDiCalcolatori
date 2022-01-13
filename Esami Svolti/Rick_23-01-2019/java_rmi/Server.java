import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.util.ArrayList;
import java.util.List;

public class Server extends UnicastRemoteObject implements IServer {

  private Prenotazione[] prenotazioni;
  private final int N = 10;

  protected Server() throws RemoteException {
    super();
    prenotazioni = new Prenotazione[N];
    for (int i = 0; i < N; i++) {
      prenotazioni[i] = new Prenotazione();
    }
  }

  // restituisce -1 in caso di errore, e l'indice in caso di successo
  public int elimina_prenotazione(String targa) throws RemoteException {
    for (int i = 0; i < N; i++) {
      if (prenotazioni[i].targa.equals(targa)) {
        prenotazioni[i] = new Prenotazione();
        return i;
      }
    }
    return -1;
  }

  public List<Prenotazione> visualizza_prenotazioni() throws RemoteException {
    List<Prenotazione> _prenotazioni = new ArrayList<Prenotazione>();
    char ch1, ch2;
    for (int i = 0; i < N; i++) {
      if (prenotazioni[i].isLibera())
        continue;
      ch1 = prenotazioni[i].targa.charAt(0);
      ch2 = prenotazioni[i].targa.charAt(1);
      if ((ch1 < 'E') || (ch1 == 'E' && ch2 < 'D'))
        continue;

      _prenotazioni.add(prenotazioni[i]);
    }
    return _prenotazioni;
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
