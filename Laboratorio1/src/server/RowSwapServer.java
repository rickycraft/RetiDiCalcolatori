package server;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.SocketException;
import java.util.StringTokenizer;

public class RowSwapServer extends UdpServer implements Runnable {

	public String filename;

	public RowSwapServer(int port, String filename) throws SocketException {
		super(port);
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
				// salvo ind e porta da dove � arrivato il pacchetto
				InetAddress clientAddress = packet.getAddress();
				int clientPort = packet.getPort();
				// preparo il pacchetto da spedire
				byte[] buf = new byte[256];
				packet = new DatagramPacket(buf, buf.length, clientAddress, clientPort);
				// send to client
				byteOut = new ByteArrayOutputStream();
				dataOut = new DataOutputStream(byteOut);
				dataOut.writeInt(reply);
				packet.setData(byteOut.toByteArray());
				listenSocket.send(packet);
			} catch (IOException e) {
				// TODO handle exception
				e.printStackTrace();
			} catch (NumberFormatException e) {
				// TODO handle exception
				e.printStackTrace();
			}
		}
	}

}
