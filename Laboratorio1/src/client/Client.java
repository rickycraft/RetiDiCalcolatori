package client;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
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

			byte[] buff = new byte[256];
			DatagramSocket socket = new DatagramSocket();
			DatagramPacket packet = new DatagramPacket(buff, buff.length, address, port);

			// send discovery packet
			ByteArrayOutputStream byteOut = new ByteArrayOutputStream();
			DataOutputStream dataOut = new DataOutputStream(byteOut);
			dataOut.writeUTF(filename);
			packet.setData(byteOut.toByteArray());
			socket.send(packet);

			// get discovery packet
			socket.receive(packet);
			ByteArrayInputStream byteIn = new ByteArrayInputStream(packet.getData(), 0, packet.getLength());
			DataInputStream dataIn = new DataInputStream(byteIn);
			String rsPort = dataIn.readUTF();

			// request to RowSwapServer
			// packet = new DatagramPacket(buff, buff.length, address,
			// Integer.parseInt(rsPort));

			// wait for reply
		} catch (IOException e) {
			e.printStackTrace();
		} catch (NumberFormatException e) {
			e.printStackTrace();
		}
	}

}
