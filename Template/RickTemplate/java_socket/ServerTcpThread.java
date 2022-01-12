import java.io.*;
import java.net.*;

public class ServerTcpThread extends Thread {
  private Socket clientSocket = null;

  public ServerTcpThread(Socket socket) {
    this.clientSocket = socket;
  }

  @Override
  public void run() {
    DataInputStream inSock;
    DataOutputStream outSock;

    // CREAZIONE STREAM INPUT E OUTPUT
    try {
      inSock = new DataInputStream(clientSocket.getInputStream());
      outSock = new DataOutputStream(clientSocket.getOutputStream());
    } catch (IOException ioe) {
      System.out.println("server# Problemi nella creazione degli stream di input/output ");
      ioe.printStackTrace();
      return;
    }
    System.out.println("server# Avviato " + getName());
    // CORPO DELLA THREAD
    try {
      while (true) {
        String response = inSock.readUTF();
        System.out.println("server# ricevuto " + response);
        outSock.writeUTF(response);
      }
    } catch (EOFException e) {
      System.out.println("server# Terminazione. " + getName());
    } catch (SocketTimeoutException e) {
      System.out.println("server# Socket timed out. " + getName());
      e.printStackTrace();
    } catch (IOException e) {
      System.out.println("server# Lettura/Scrittura su stream fallita. " + getName());
      e.printStackTrace();
    } catch (Exception e) {
      System.out.println("server# Errore irreversibile. " + getName());
      e.printStackTrace();
    } finally {
      try {
        clientSocket.close();
        System.out.println("server# Chiusa socket. " + getName());
      } catch (IOException e) {
        System.out.println("server# Chiusura socket fallita. " + getName());
      }
    }
  }
}
