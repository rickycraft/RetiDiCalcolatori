package client;

import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
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

			if (port < 1024 || port > 65535) {
				System.out.println("Porta non valida");
				System.exit(1);
			}
			// creazione datagramSocket nel cliente
			DatagramSocket socket = new DatagramSocket();
			System.out.println(socket.getLocalPort());
			System.out.println(socket.getLocalAddress());
			System.out.println(InetAddress.getLocalHost());
			socket.setSoTimeout(30000);

			// creazione pacchetto datagram con nomeFile
			byte[] buf = new byte[256];
			DatagramPacket packet = new DatagramPacket(buf, buf.length, address, port);
			ByteArrayOutputStream byteOutStream = new ByteArrayOutputStream();
			DataOutputStream dataOutStream = new DataOutputStream(byteOutStream);
			dataOutStream.writeUTF(filename);
			byte[] sendData = byteOutStream.toByteArray();
			packet.setData(sendData);
			// invio del pacchetto al server
			socket.send(packet);

			// inizializzaione del pacchetto risposta
			packet.setData(buf);
			socket.receive(packet); // attesa del pacchetto di risposta

			// estrazione del pacchetto di ritorno
			ByteArrayInputStream byteInStream = new ByteArrayInputStream(packet.getData(), 0, packet.getLength());
			DataInputStream dataInStream = new DataInputStream(byteInStream);
			// risposta del Discover Server ( intero )
			int rsPort = dataInStream.readInt();
			// caso esito negativo da DS
			if (rsPort < 1024 || rsPort > 65535) {
				System.out.println("errore: file non presente");
				socket.close();
				System.exit(1);
			}

			// caso esito positivo posito ( numero porta)

			packet = new DatagramPacket(buf, buf.length, address, rsPort);
			BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));
			System.out.println("ctrl Z o ctrl D per uscire, altrimenti numero di riga 1");
			String letto;
			while ((letto = stdIn.readLine()) != null) {
				System.out.println("numero di riga 2");
				int numLinea1 = Integer.parseInt(letto);
				int numLinea2 = Integer.parseInt(stdIn.readLine());
				String numeri = numLinea1 + " " + numLinea2; // numero1+space+numero2
				byteOutStream = new ByteArrayOutputStream();
				dataOutStream = new DataOutputStream(byteOutStream);
				dataOutStream.writeUTF(numeri);
				sendData = byteOutStream.toByteArray();

				packet.setData(sendData);
				socket.send(packet);
				// aspetto il pacchetto di ritorno
				packet.setData(buf);
				socket.receive(packet); // attesa del pacchetto di risposta

				// estrazione del pacchetto di ritorno
				byteInStream = new ByteArrayInputStream(packet.getData(), 0, packet.getLength());
				dataInStream = new DataInputStream(byteInStream);
				// risposta del RowSwap Server ( intero )
				int rispRS = dataInStream.readInt();
				// caso esito negativo da DS

				if (rispRS < 0) {
					System.out.println("errore: righe non scambiate");
				}
				else
					System.out.println("Lo scambio di righe � avvenuto con successo");
				System.out.println(" ctrl Z o ctrl D per uscire, altrimenti numero di riga 1");
			}

			// finito il ciclo delle richieste
			socket.close();
			System.out.println("ho finito");

			System.exit(0);
		} catch (IOException e) {
			e.printStackTrace();
		} catch (NumberFormatException e) {
			e.printStackTrace();
		}
	}

}
