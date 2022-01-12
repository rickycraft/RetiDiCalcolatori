import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.util.List;

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
      System.out
          .print("client# Scegli il servizio che vuoi usare 1(elimina prenotazione)/2(visualizza prenotazione): ");
      while ((service = stdIn.readLine()) != null) {
        if (service.equals("1")) {
          System.out.print("client# Inserisci il numero di targa ");
          service = stdIn.readLine();
          if (serverRMI.elimina_prenotazione(service) < 0) {
            System.out.println("client# Non sono riuscito ad eliminare la prenotazione");
          } else {
            System.out.println("client# Eliminata la prenotazione");
          }
        } else if (service.equals("2")) {
          List<Prenotazione> prenotazioni = serverRMI.visualizza_prenotazioni();
          for (Prenotazione prenotazione : prenotazioni) {
            System.out.println("client# Prenotazione " + prenotazione.toString());
          }
        } else {
          System.out.println("client# Il codice di servizio non è valido");
        }
        System.out
            .print("client# Scegli il servizio che vuoi usare 1(elimina prenotazione)/2(visualizza prenotazione): ");
        ;
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
