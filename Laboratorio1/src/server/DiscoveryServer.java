package server;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.SocketException;
import java.util.ArrayList;
import java.util.List;

public class DiscoveryServer extends UdpServer {

	private static final String USAGE = "Usage: java DiscoveryServer [serverPort>1024] nome1 porta1 nome2 porta2";

	// ottimizzazione creazione di un array
	private List<RowSwapServer> rsServers;

	public DiscoveryServer(int port, List<RowSwapServer> servers) throws SocketException {
		super(port);
		this.rsServers = servers;
	}

	private void startServers() {
		for (RowSwapServer rs : rsServers) {
			new Thread(rs).start();
		}
	}

	private void start() {
		String data;
		int reply;
		while (true) {
			// set -1 as an error
			reply = -1;
			try {
				System.out.println("Porta DS:"+listenSocket.getLocalPort());
				System.out.println("IP DS:" + listenSocket.getLocalAddress());
				listenSocket.receive(packet);
				System.out.println("Ricevuto packet");
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
				System.out.println("Spedito: "+reply);
				listenSocket.send(packet);
			} catch (IOException e) {
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
			List<RowSwapServer> servers = new ArrayList<>();
			for (int i = 1; i < arg.length; i += 2) {
				// args[i] nomeFile
				// args[i+1] porta
				// controllo dei duplicati, sia file che porta
				int port = Integer.parseInt(arg[i + 1]);
				servers.add(new RowSwapServer(port, arg[i]));
			}
			ds = new DiscoveryServer(portDS, servers);
			ds.startServers(); // Avvio tutti i server RowSwap
			ds.start(); // Avvio il server DS

			// catch errors
		} catch (SocketException e) {
			System.out.println("Socket exception");
			System.exit(2);
		} catch (NumberFormatException e) {
			System.out.println(USAGE);
			System.exit(1);
		}

	}

}
