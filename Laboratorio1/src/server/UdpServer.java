package server;

import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;

public class UdpServer {

	protected DatagramSocket listenSocket;
	protected DatagramPacket packet;
	byte[] buffer;
	protected int port;

	DataInputStream dataIn;
	DataOutputStream dataOut;
	ByteArrayOutputStream byteOut;

	public UdpServer(int port) throws SocketException {
		// controllo argomento e che la porta sia nel range consentito 1024-65535
		if (port < 1024 || port > 65535) {
			System.out.println("Invalid port range");
			System.exit(1);
		}

		this.port = port;
		listenSocket = new DatagramSocket(port);
		buffer = new byte[256];
		packet = new DatagramPacket(buffer, buffer.length);
		packet.setData(buffer);
	}

}
