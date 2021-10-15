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

	public static void main(String args[]) {
		// args check
		if (args.length != 3) {
			System.exit(1);
		}

		try {
			// variabili IPDS portDS fileName
			InetAddress address = InetAddress.getByName(args[0]);
			// System.out.println(InetAddress.getLocalHost().toString());
			int port = Integer.parseInt(args[1]);
			String filename = args[2];

			if (port < 1024 || port > 65535) {
				System.out.println("Porta non valida");
				System.exit(1);
			}

			// creazione datagramSocket
			DatagramSocket socket = new DatagramSocket();
			System.out.println(socket.getLocalPort());
			System.out.println(socket.getLocalAddress());
			socket.setSoTimeout(300000);
			byte[] buff = new byte[256];
			// creazione pacchetto datagram
			DatagramPacket packet = new DatagramPacket(buff, buff.length);
			packet.setAddress(address);
			packet.setPort(port);
			// send discovery packet
			// creazione del pacchetto con le informazione del nome file
			ByteArrayOutputStream byteOutStream = new ByteArrayOutputStream();
			DataOutputStream dataOutStream = new DataOutputStream(byteOutStream);
			dataOutStream.writeUTF(filename); // nomeFile una stringa
			byte[] data = byteOutStream.toByteArray();
			// riempimento del pacchetto
			packet.setData(data, 0, data.length);
			// invio del pacchetto al server
			socket.send(packet);
			// scatta IllegalArgumentException perche server non attivo

			// inizializzaione del pacchetto risposta
			packet.setData(buff);
			// attesa del pacchetto di risposta
			socket.receive(packet);
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
			// chiudo il socket DS
			socket.close();
			char si_no;
			DatagramSocket socketRS = new DatagramSocket(rsPort, address);

			do {
				// chiedo da terminale al cliente i numeri delle due righe
				System.out.println("Inserisci il numero delle righe da scambiare");
				BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));
				String righeStr = stdIn.readLine();
				int numLinea1 = Integer.parseInt((righeStr.split(" "))[0]);
				int numLinea2 = Integer.parseInt((righeStr.split(" "))[1]);
				/*
				 * System.out.println("numero di riga 1"); int numLinea1 =
				 * Integer.parseInt(stdIn.readLine()); System.out.println("numero di riga 2");
				 * int numLinea2 = Integer.parseInt(stdIn.readLine());
				 */

				// creazione datagramSocket RS (stesso indirizzo , diversa porta)
				socket.setSoTimeout(300000);
				byte[] buffRS = new byte[256];
				// creazione pacchetto datagram RS
				DatagramPacket packetRS = new DatagramPacket(buffRS, buffRS.length);

				// send RowSwap packet
				// creazione del pacchetto con le informazione di NumRiga
				// ???D
				// ByteArrayOutputStream byteOutStream = new ByteArrayOutputStream();
				// DataOutputStream dataOutStream = new DataOutputStream(byteOutStream);
				dataOutStream.writeInt(numLinea1); // numRiga1
				dataOutStream.writeInt(numLinea2); // numRiga2
				byte[] dataRS = byteOutStream.toByteArray();
				// riempimento del pacchetto RS
				packetRS.setData(dataRS, 0, dataRS.length);
				// invio del pacchetto al server RS
				socketRS.send(packetRS);

				// inizializzaione del pacchetto risposta
				packet.setData(buffRS);
				// attesa del pacchetto di risposta
				socketRS.receive(packetRS);
				// estrazione del pacchetto di ritorno
				// ByteArrayInputStream byteInStream = new
				// ByteArrayInputStream(packet.getData(), 0, packet.getLength());
				// DataInputStream dataInStream = new DataInputStream(byteInStream);
				// risposta del Row Swap Server ( intero )
				int rispRS = dataInStream.readInt();
				// caso esito negativo da RS
				if (rispRS < 0) {
					System.out.println("errore: righe non scambiate");
				}

				// chiedo al cliente se vuole fare una nuova richiesta
				System.out.println("nuova richiesta??? (Y/N) ");
				si_no = stdIn.readLine().charAt(0);
			} while (si_no == 'y' || si_no == 'Y');
			// finito il ciclo delle richieste
			System.out.println("ho finito");
			socketRS.close();
			System.exit(0);
		} catch (IOException e) {
			e.printStackTrace();
		} catch (NumberFormatException e) {
			e.printStackTrace();
		}
	}

}
