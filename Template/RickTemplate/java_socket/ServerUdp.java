import java.io.*;
import java.net.*;

public class ServerUdp {

  public static void main(String[] args) {

    DatagramSocket socket = null;
    DatagramPacket packet = null;
    byte[] buf = new byte[256];
    int port = -1;

    // controllo argomenti input: 0 oppure 1 argomento (porta)
    if (args.length != 1) {
      System.out.println("server# args.length != 1");
      System.exit(1);
    }
    try {
      port = Integer.parseInt(args[0]);
      // controllo che la porta sia nel range consentito 1024-65535
      if (port < 1024 || port > 65535) {
        System.out.println("server# Porta fuori dal range");
        System.exit(1);
      }
    } catch (NumberFormatException e) {
      System.out.println("server# Fallito parsing porta");
      System.exit(1);
    }

    try {
      socket = new DatagramSocket(port);
      packet = new DatagramPacket(buf, buf.length);
      System.out.println("server# Creata la socket: " + socket.toString());
    } catch (SocketException e) {
      System.out.println("server# Problemi nella creazione della socket: ");
      System.exit(1);
    }
    System.out.println("#server Avviato");

    try {
      String richiesta = null;
      ByteArrayInputStream biStream = null;
      DataInputStream diStream = null;
      ByteArrayOutputStream boStream = null;
      DataOutputStream doStream = null;
      byte[] data = null;

      while (true) {
        // ricezione del datagramma
        try {
          packet.setData(buf);
          socket.receive(packet);
        } catch (IOException e) {
          System.err.println("Problemi nella ricezione del datagramma: " + e.getMessage());
          continue;
        }
        // ricevo il pacchetto
        try {
          biStream = new ByteArrayInputStream(packet.getData(), 0, packet.getLength());
          diStream = new DataInputStream(biStream);
          richiesta = diStream.readUTF();
          System.out.println("server# Ricevuta richiesta: " + richiesta);
        } catch (Exception e) {
          System.err.println("server# Problemi nella lettura della richiesta");
          continue;
        }
        // rispondo
        try {
          boStream = new ByteArrayOutputStream();
          doStream = new DataOutputStream(boStream);
          String risposta = richiesta;
          doStream.writeUTF(risposta);
          data = boStream.toByteArray();
          packet.setData(data, 0, data.length);
          socket.send(packet);
        } catch (IOException e) {
          System.err.println("server# Problemi nell'invio della risposta");
          continue;
        }
      }
    } catch (Exception e) {
      e.printStackTrace();
    }
    System.out.println("server# Termino");
    socket.close();
  }
}
