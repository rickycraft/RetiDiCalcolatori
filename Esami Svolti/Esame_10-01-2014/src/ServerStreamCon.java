

import java.net.ServerSocket;
import java.net.Socket;

public class ServerStreamCon {

  public static final int DF_PORT = 1050; // default port
  private static final int MAX_DATI = 10;
  private static String[][] dati = new String[MAX_DATI][6]; //va bene string che tanto devo solo visualizzare e non modificare dati

  public static void main(String[] args) {
    int port = -1;

    //CONTROLLO ARGOMENTI
    try {
      if (args.length == 1) {
        port = Integer.parseInt(args[0]);
        if (port < 1024 || port > 65535) {
          System.out.println(
            "[ERROR] la porta deve essere compresa nel range valido [1024-65535]"
          );
          System.exit(1);
        }
      } else if (args.length == 0) {
        System.out.println("[DEBUG] Server avviato con porta a default");
        port = DF_PORT;
      } else {
        System.out.println("[ERROR] parametro errato <porta>");
        System.exit(1);
      }
    } catch (Exception e) { //try
      System.out.println("Problemi, i seguenti: ");
      e.printStackTrace();
      System.err.println("Java Server porta");
      System.exit(1);
    }

    //INIZIALIZZAZIONE STRUTTURA DATI INTERNA
    for (int i = 0; i < MAX_DATI; i++) {
      for (int j = 0; j < 6; j++) {
        dati[i][j] = "L"; //id tipo luogo data biglietti prezzo
      }
    }
    dati[0][0] = "prova1";
    dati[0][1] = "Formula1";
    dati[0][2] = "Roma";
    dati[0][3] = "12/12/1212";
    dati[0][4] = "256";
    dati[0][5] = "200";

    dati[1][0] = "prova2";
    dati[1][1] = "Calcio";
    dati[1][2] = "Torriana";
    dati[1][3] = "12/12/1212";
    dati[1][4] = "2";
    dati[1][5] = "15";

    dati[2][0] = "prova3";
    dati[2][1] = "Concerto";
    dati[2][2] = "Bologna";
    dati[2][3] = "12/12/1212";
    dati[2][4] = "9";
    dati[2][5] = "9";

    //AVVIO SERVER

    ServerSocket serverSocket = null;
    Socket clientSocket = null;

    try {
      serverSocket = new ServerSocket(port);
      serverSocket.setReuseAddress(true);
      System.out.println("[DEBUG] Server: avviato ");
      System.out.println(
        "[DEBUG] Server: creata la server socket: " + serverSocket
      );
    } catch (Exception e) {
      System.err.println(
        "[ERROR] Server: problemi nella creazione della server socket: " +
        e.getMessage()
      );
      e.printStackTrace();
      System.exit(1);
    }

    //CICLO DI ATTESA RICHIESTE
    try {
      while (true) {
        System.out.println("[DEBUG] Server: in attesa di richieste...\n");
        try {
          // bloccante fino ad una pervenuta connessione
          clientSocket = serverSocket.accept();
          clientSocket.setSoTimeout(300000); //un timeout
          System.out.println(
            "[DEBUG]Server: connessione accettata: " + clientSocket
          );
        } catch (Exception e) {
          System.err.println(
            "[ERROR]Server: problemi nella accettazione della connessione: " +
            e.getMessage()
          );
          e.printStackTrace();
          continue;
        }

        // servizio delegato ad un nuovo thread passando il clientSocket
        try {
          new StreamThread(clientSocket, dati).start();
        } catch (Exception e) {
          System.err.println(
            "[ERROR]Server: problemi nel server thread: " + e.getMessage()
          );
          e.printStackTrace();
          continue;
        }
      } // while ciclo di attese
    } // Eccezioni non catturate dentro il while
    catch (Exception e) {
      System.out.println("[ERROR] Server: esplodo...");
      e.printStackTrace();
      System.exit(2);
    }
  }
}
