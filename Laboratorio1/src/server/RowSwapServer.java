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
import java.util.StringTokenizer;

public class RowSwapServer implements Runnable {

	public String filename;

	protected DatagramSocket listenSocket;
	protected DatagramPacket packet;
	byte[] buffer;
	protected int port;

	DataInputStream dataIn;
	DataOutputStream dataOut;
	ByteArrayOutputStream byteOut;

	public RowSwapServer(int port, String filename) {
		// controllo argomento e che la porta sia nel range consentito 1024-65535
		if (port < 1024 || port > 65535) {
			System.out.println("Invalid port range");
			System.exit(1);
		}

		this.port = port;
		try {
			listenSocket = new DatagramSocket(port, InetAddress.getLocalHost());
		} catch (SocketException | UnknownHostException e) {
			System.out.println("Errore durante il lancio del rowswap server");
			e.printStackTrace();
			System.exit(1);
		}
		buffer = new byte[256];
		packet = new DatagramPacket(buffer, buffer.length);
		packet.setData(buffer);

		this.filename = filename;
	}

	@Override
	public void run() {
		String data;
		int reply;
		int line1, line2;
		while (true) {
			try {
				packet.setData(buffer);
				listenSocket.receive(packet);
				data = new DataInputStream(new ByteArrayInputStream(packet.getData(), 0, packet.getLength())).readUTF();
				StringTokenizer tk = new StringTokenizer(data);
				line1 = Integer.parseInt(tk.nextToken());
				line2 = Integer.parseInt(tk.nextToken());

				// controllo dello stato e stampa a video
				reply = LineUtility.swapLine(filename, line1, line2);
				// preparo il pacchetto da spedire
				packet = new DatagramPacket(buffer, buffer.length, packet.getAddress(), packet.getPort());
				// send to client
				byteOut = new ByteArrayOutputStream();
				dataOut = new DataOutputStream(byteOut);
				dataOut.writeInt(reply);
				packet.setData(byteOut.toByteArray());
				listenSocket.send(packet);
			} catch (IOException e) {
				System.out.println("Errore IO sul pacchetto");
				e.printStackTrace();
			} catch (NumberFormatException e) {
				System.out.println("Errore lettura delle linee");
				e.printStackTrace();
			}
		}
	}

}
