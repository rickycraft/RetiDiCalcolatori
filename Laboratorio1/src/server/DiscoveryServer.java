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
				listenSocket.receive(packet);
				data = new DataInputStream(new ByteArrayInputStream(packet.getData(), 0, packet.getLength())).readUTF();
				// find the server
				for (RowSwapServer rs : rsServers) {
					if (rs.filename == data) {
						reply = rs.port;
						break;

					}
				}
				// send to client
				byteOut = new ByteArrayOutputStream();
				dataOut = new DataOutputStream(byteOut);
				dataOut.writeUTF(String.valueOf(reply));
				packet.setData(byteOut.toByteArray());
				listenSocket.send(packet);
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	public static void main(String[] args) {
		DiscoveryServer ds;

		// controllo che siano dispari, coppie per i rowswap + porta del discovery
		if (((args.length % 2) == 0) || (args.length < 3)) {
			System.out.println("Usage: java DiscoveryServer portaDS nomeFile1 Porta1 [... nomeFileN PortaN]");
			System.exit(3);
		}

		try {
			// DiscoveryServer port
			int port = Integer.parseInt(args[0]);
			List<RowSwapServer> servers = new ArrayList<>();
			for (int i = 1; i < args.length; i += 2) {
				// args[i] nomeFile
				// args[i+1] porta
				// controllo dei duplicati, sia file che porta
				port = Integer.parseInt(args[i + 1]);
				servers.add(new RowSwapServer(port, args[i]));
			}
			ds = new DiscoveryServer(port, servers);
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
