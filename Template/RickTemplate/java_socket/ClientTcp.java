import java.io.*;
import java.net.*;

public class ClientTcp {

  public static void main(String[] args) {

    InetAddress addr = null;
    int port = -1;
    Socket socket = null;
    DataInputStream inSock = null;
    DataOutputStream outSock = null;
    BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));
    String buffer, response;

    if (args.length != 2) {
      System.out.println("client# java args.length != 2");
      System.exit(1);
    }
    try {
      addr = InetAddress.getByName(args[0]);
      port = Integer.parseInt(args[1]);
      if (port < 1024 || port > 65535) {
        System.out.println("client# la porta deve essere compresa nel range valido");
        System.exit(1);
      }
    } catch (IOException e) {
      System.out.println("client# io exception");
      System.exit(1);
    }

    // CREAZIONE SOCKET
    try {
      socket = new Socket(addr, port);
      socket.setSoTimeout(300000);
      System.out.println("client# Creata " + socket);
    } catch (Exception e) {
      System.out.println("client# Problemi nella creazione della socket: ");
      System.exit(1);
    }

    // CREAZIONE STREAM
    try {
      inSock = new DataInputStream(socket.getInputStream());
      outSock = new DataOutputStream(socket.getOutputStream());
    } catch (IOException e) {
      System.out.println("client# Problemi nella creazione degli stream su socket: ");
      System.exit(1);
    }

    // CICLO INTERAZIONE CLIENT
    System.out.print("Inserisci il testo da mandare: ");
    try {
      while ((buffer = stdIn.readLine()) != null) {
        outSock.writeUTF(buffer);
        response = inSock.readUTF();
        System.out.println("client# Ricevuta response: " + response);
        System.out.print("Inserisci il testo da mandare: ");
      }
    } catch (Exception e) {
      System.out.println("client# Problemi nel ciclo di interazione");
    }

    // CHIUSURA SOCKET
    try {
      socket.shutdownOutput();
      socket.shutdownInput();
      socket.close();
    } catch (IOException e) {
      System.out.println("client# Problemi nella chiusura delle socket");
      System.exit(1);
    }
    System.out.println("client# Termino");
  }

}
