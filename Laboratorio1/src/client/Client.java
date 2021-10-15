package client;

import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class Client {

	public static void main(String[] args) {
		// args check
		if (args.length != 3) {
			System.exit(1);
		}

		try {
			// variabili IPDS portDS fileName
			InetAddress address = InetAddress.getByName(args[0]);
			int port = Integer.parseInt(args[1]);
			String filename = args[2];

			// creazione datagramSocket nel cliente
			DatagramSocket socket = new DatagramSocket();
			socket.setSoTimeout(3000);

			// creazione pacchetto datagram con nomeFile
			byte[] sentData = new byte[256];
			sentData = filename.getBytes();
			DatagramPacket packet = new DatagramPacket(sentData, sentData.length, address, port);
			// invio del pacchetto al server
			socket.send(packet);

			// inizializzaione del pacchetto risposta
			byte[] receiveData = new byte[256];
			packet = new DatagramPacket(receiveData, receiveData.length);
			socket.receive(packet); // attesa del pacchetto di risposta

			// estrazione del pacchetto di ritorno
			ByteArrayInputStream byteInStream = new ByteArrayInputStream(packet.getData(), 0, packet.getLength());
			DataInputStream dataInStream = new DataInputStream(byteInStream);
			// risposta del Discover Server ( intero )
			int rsPort = dataInStream.readInt();
			// caso esito negativo da DS
			if (rsPort < 0) {
				System.out.println("errore: file non presente");
				socket.close();
				System.exit(1);
			}

			// caso esito positivo posito ( numero porta)
			BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));
			System.out.println("numero di riga 1");
			String numLinea1;

			while ((numLinea1 = stdIn.readLine()) != null) {
				System.out.println("numero di riga 2");
				String numLinea2 = stdIn.readLine();
				byte[] sentData2 = new byte[256];
				sentData2 = numLinea1.getBytes();
				sentData2 = numLinea2.getBytes();
				packet = new DatagramPacket(sentData2, sentData2.length, address, rsPort);
				socket.send(packet);

				byte[] receiveData2 = new byte[256];
				packet = new DatagramPacket(receiveData2, receiveData2.length);
				socket.receive(packet);

				byteInStream = new ByteArrayInputStream(packet.getData(), 0, packet.getLength());
				dataInStream = new DataInputStream(byteInStream);
				// risposta del RowSwap Server ( intero )
				int rispRS = dataInStream.readInt();
				if (rispRS < 0) {
					System.out.println("errore: righe non scambiate");
				}
				System.out.println(" numero di riga 1");
				numLinea1 = stdIn.readLine();
			}

			// finito il ciclo delle richieste
			System.out.println("ho finito");
			socket.close();
			System.exit(0);
		} catch (IOException e) {
			e.printStackTrace();
		} catch (NumberFormatException e) {
			e.printStackTrace();
		}
	}

}
