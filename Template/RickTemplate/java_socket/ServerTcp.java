import java.net.ServerSocket;
import java.net.Socket;

public class ServerTcp {

  public static void main(String[] args) {

    int port = -1;

    // CONTROLLO ARGOMENTI
    if (args.length != 1) {
      System.out.println("server# parametro errato args.length != 1");
      System.exit(1);
    }
    port = Integer.parseInt(args[0]);
    if (port < 1024 || port > 65535) {
      System.out.println("server# la porta deve essere compresa nel range valido [1024-65535]");
      System.exit(1);
    }

    // AVVIO SERVER
    ServerSocket serverSocket = null;
    Socket clientSocket = null;
    try {
      serverSocket = new ServerSocket(port);
      serverSocket.setReuseAddress(true);
      System.out.println("server# creata " + serverSocket);
    } catch (Exception e) {
      System.out.println("server# problemi nella creazione della server socket: " + e.getMessage());
      System.exit(1);
    }
    System.out.println("server# avviato e in attesa di richieste");

    // CICLO DI ATTESA RICHIESTE
    try {
      while (true) {
        // bloccante fino ad una pervenuta connessione
        try {
          clientSocket = serverSocket.accept();
          clientSocket.setSoTimeout(300000);
          System.out.println("server# connessione accettata: " + clientSocket);
        } catch (Exception e) {
          System.out.println("server# problemi nella accettazione della connessione: " + e.getMessage());
          continue;
        }
        // servizio delegato ad un nuovo thread passando il clientSocket
        try {
          new ServerTcpThread(clientSocket).start();
        } catch (Exception e) {
          System.out.println("server# problemi nel server thread: " + e.getMessage());
          continue;
        }
      } // while ciclo di attese
    }
    // Eccezioni non catturate dentro il while
    catch (Exception e) {
      System.out.println("server# Errore generico: " + e.getMessage());
      System.exit(1);
    }
  }
}
