package server;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

public class DiscoveryServer {

	private static final String USAGE = "Usage: java DiscoveryServer [serverPort>1024] nome1 porta1 nome2 porta2";

	// ottimizzazione creazione di un array
	private RowSwapServer[] rsServers;

	protected DatagramSocket listenSocket;
	protected DatagramPacket packet;
	byte[] buffer;
	protected int port;

	DataInputStream dataIn;
	DataOutputStream dataOut;
	ByteArrayOutputStream byteOut;

	public DiscoveryServer(int port, RowSwapServer[] servers) {
		// controllo argomento e che la porta sia nel range consentito 1024-65535
		if (port < 1024 || port > 65535) {
			System.out.println("Invalid port range");
			System.exit(1);
		}

		this.port = port;
		try {
			listenSocket = new DatagramSocket(port, InetAddress.getLocalHost());
		} catch (SocketException | UnknownHostException e) {
			System.out.println("Errore durante il lancio del discovery server");
			e.printStackTrace();
			System.exit(1);
		}
		buffer = new byte[256];
		packet = new DatagramPacket(buffer, buffer.length);
		packet.setData(buffer);
		rsServers = servers;
	}

	private void startServers() {
		for (RowSwapServer rs : rsServers) {
			new Thread(rs).start();
		}
	}

	private void start() {
		String data;
		int reply;
		System.out.printf("Started at %s:%s", listenSocket.getLocalAddress(), listenSocket.getLocalPort());
		while (true) {
			// set -1 as an error
			reply = -1;
			try {
				packet.setData(buffer);
				listenSocket.receive(packet);
				data = new DataInputStream(new ByteArrayInputStream(packet.getData(), 0, packet.getLength())).readUTF();
				// find the server
				for (RowSwapServer rs : rsServers) {
					if (rs.filename.equals(data)) {
						reply = rs.port;
						break;
					}
				}
				// send to client
				byteOut = new ByteArrayOutputStream();
				dataOut = new DataOutputStream(byteOut);
				dataOut.writeInt(reply);
				packet.setData(byteOut.toByteArray());
				listenSocket.send(packet);
			} catch (IOException e) {
				System.out.println("Errore IO sul pacchetto");
				e.printStackTrace();
			}
		}
	}

	public static void main(String arg[]) {
		DiscoveryServer ds;

		// controllo che siano dispari, coppie per i rowswap + porta del discovery
		if (((arg.length % 2) == 0) || (arg.length < 3)) {
			System.out.println("Usage: java DiscoveryServer portaDS nomeFile1 Porta1 [... nomeFileN PortaN]");
			System.exit(3);
		}

		try {
			// DiscoveryServer port
			int portDS = Integer.parseInt(arg[0]);
			RowSwapServer[] servers = new RowSwapServer[(arg.length - 1) / 2];
			for (int i = 1; i < arg.length; i += 2) {
				// args[i+1] porta args[i] nomeFile
				// controllo dei duplicati, sia file che porta
				servers[(i - 1) / 2] = new RowSwapServer(Integer.parseInt(arg[i + 1]), arg[i]);
			}
			ds = new DiscoveryServer(portDS, servers);
			ds.startServers(); // Avvio tutti i server RowSwap
			ds.start(); // Avvio il server DS

		} catch (NumberFormatException e) {
			System.out.println(USAGE);
			System.exit(1);
		}

	}

}
