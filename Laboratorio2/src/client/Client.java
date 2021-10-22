// PutFileClient.java
package client;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;

import utility.FileUtility;

public class Client {
	private static final String USAGE = "Usage: java PutFileClient serverAddr serverPort directory grandezzaMinFile";

	// voglio in ingresso ip, porta , limiteMinFile e directory
	public static void main(String[] args) throws IOException {

		InetAddress addr = null;
		int port = -1;
		String directory = null;
		long maxFileSize = -1;

		try { // check args
			if (args.length != 4) {
				System.out.println(USAGE);
				System.exit(1);
			} else {
				addr = InetAddress.getByName(args[0]);
				port = Integer.parseInt(args[1]);
				directory = args[2];
				maxFileSize = Integer.parseInt(args[3]);
			}
		} catch (Exception e) {
			e.printStackTrace();
			System.out.println(USAGE);
			System.exit(2);
		}

		Socket socket = new Socket(addr, port);
		socket.setSoTimeout(300000);
		if (!socket.isConnected()) {
			System.out.println("Socket non connesso al server");
			System.exit(-1);
		}

		DataInputStream inSock = new DataInputStream(socket.getInputStream());
		DataOutputStream outSock = new DataOutputStream(socket.getOutputStream());
		FileInputStream fis;

		File dir = new File(directory);
		for (File f : dir.listFiles()) {
			if (f.isFile()) {
				if (f.length() > maxFileSize) {
					outSock.writeUTF(f.getName()); // mando il nome del file
					outSock.flush();
					System.out.printf("Mando il nome del file %s\n", f.getName());
					// ricevo risposta e scrivo il file
					if (inSock.readUTF().equalsIgnoreCase("attiva")) {
						outSock.writeLong(f.length());
						fis = new FileInputStream(f);
						System.out.print("Inizio il trasferimento del file...");
						FileUtility.trasferisci_a_byte_file_binario(new DataInputStream(fis), outSock);
						System.out.println("terminato");
						fis.close();
					} else {
						System.out.println("File non accettato dal server");
					}
				}
			}
		}
		socket.shutdownInput(); // chiusura socket (downstream)
		socket.shutdownOutput(); // chiusura socket (upstream)
		socket.close();
		System.out.println("Chiuse le connessioni, termino");

	} // main
} // PutFileClient
