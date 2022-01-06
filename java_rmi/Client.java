import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.rmi.Naming;
import java.rmi.NotBoundException;

public class Client {
  public static void main(String[] args) {

    final int REGISTRYPORT = 1099;
    String registryHost = null;
    String serviceName = "";
    BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));

    // Controllo dei parametri della riga di comando
    if (args.length != 2) {
      System.out.println("client# Sintassi: RMI_Registry_IP ServiceName");
      System.exit(1);
    }
    registryHost = args[0];
    serviceName = args[1];

    System.out.println("client# Invio richieste a " + registryHost + " per il servizio di nome " + serviceName);

    // Connessione al servizio RMI remoto
    try {
      String completeName = "//" + registryHost + ":" + REGISTRYPORT + "/" + serviceName;
      IServer serverRMI = (IServer) Naming.lookup(completeName);
      System.out.println("client# ClientRMI: Servizio \"" + serviceName + "\" connesso");

      String service;
      System.out.print("client# Inserisci la nuova proprietà: ");
      while ((service = stdIn.readLine()) != null) {
        System.out.println(serverRMI.setProp(service));
        serverRMI.oggetto().stampa();
        System.out.print("client# Inserisci la nuova proprietà: ");
      } // while (!EOF), fine richieste utente

    } catch (NotBoundException nbe) {
      System.err.println("client# ClientRMI: il nome fornito non risulta registrato; " + nbe.getMessage());
      nbe.printStackTrace();
      System.exit(1);
    } catch (Exception e) {
      System.err.println("client# ClientRMI: " + e.getMessage());
      e.printStackTrace();
      System.exit(1);
    }
  }
}
