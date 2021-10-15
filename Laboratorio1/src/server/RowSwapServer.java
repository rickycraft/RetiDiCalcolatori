package server;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
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
				listenSocket.receive(packet);
				data = new DataInputStream(new ByteArrayInputStream(packet.getData(), 0, packet.getLength())).readUTF();
				StringTokenizer tk = new StringTokenizer(data);
				line1 = Integer.parseInt(tk.nextToken());
				line2 = Integer.parseInt(tk.nextToken());

				// controllo dello stato e stampa a video
				reply = LineUtility.swapLine(filename, line1, line2);

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
