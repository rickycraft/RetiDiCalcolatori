import java.io.*;
import java.net.*;

public class ClientUdp {

  public static void main(String[] args) {

    InetAddress addr = null;
    int port = -1;

    try {
      if (args.length == 2) {
        addr = InetAddress.getByName(args[0]);
        port = Integer.parseInt(args[1]);
      } else {
        System.out.println("client# args.length == 2");
        System.exit(1);
      }
    } catch (UnknownHostException e) {
      System.out.println("client# Problemi nella determinazione dell'endpoint");
      System.exit(1);
    }

    DatagramSocket socket = null;
    DatagramPacket packet = null;
    byte[] buf = new byte[256];

    // creazione della socket datagram
    try {
      socket = new DatagramSocket();
      socket.setSoTimeout(30000);
      packet = new DatagramPacket(buf, buf.length, addr, port);
      System.out.println("client# Creata la socket: " + socket);
    } catch (SocketException e) {
      System.out.println("client# Problemi nella creazione della socket:");
      System.exit(1);
    }
    System.out.println("client# Avviato");

    BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));
    System.out.print("client# Inserisci il testo da mandare: ");

    try {
      ByteArrayOutputStream boStream = null;
      DataOutputStream doStream = null;
      byte[] data = null;
      String richiesta = null;
      String risposta = null;
      ByteArrayInputStream biStream = null;
      DataInputStream diStream = null;

      while ((richiesta = stdIn.readLine()) != null) {

        // riempimento e invio del pacchetto
        try {
          boStream = new ByteArrayOutputStream();
          doStream = new DataOutputStream(boStream);
          doStream.writeUTF(richiesta);
          data = boStream.toByteArray();
          packet.setData(data);
          socket.send(packet);
          System.out.println("client# Richiesta inviata");
        } catch (IOException e) {
          System.out.println("client# Problemi nell'invio della richiesta: ");
          System.out.print("client# Inserisci il testo da mandare: ");
          continue;
        }

        // ricezione del pacchetto
        try {
          // settaggio del buffer di ricezione
          packet.setData(buf);
          socket.receive(packet);
          biStream = new ByteArrayInputStream(packet.getData(), 0, packet.getLength());
          diStream = new DataInputStream(biStream);
          risposta = diStream.readUTF();
          System.out.println("client# Risposta: " + risposta);
        } catch (IOException e) {
          System.out.println("client# Problemi nella ricezione della richiesta: ");
          System.out.print("client# Inserisci il testo da mandare: ");
          continue;
        }

        System.out.print("client# Inserisci il testo da mandare: ");
      } // while
    } catch (Exception e) {
      e.printStackTrace();
    }
    System.out.println("client# Termina client");
    socket.close();
  }
}