// PutFileServer Concorrente
package server;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class PutFileServerCon {
	public static int port = 6000; // default port

	public static void main(String[] args) throws IOException {

		/* controllo argomenti */
		try {
			if (args.length == 1) {
				port = Integer.parseInt(args[0]);
				if (port < 1024 || port > 65535) {
					System.out.println("Usage: java LineServer [serverPort>1024]");
					System.exit(1);
				}
			} else if (args.length == 0) {
			} else {
				System.out.println("Usage: java PutFileServerThread or java PutFileServerThread port");
				System.exit(1);
			}
		} // try
		catch (Exception e) {
			System.out.println("Problemi, i seguenti: ");
			e.printStackTrace();
			System.out.println("Usage: java PutFileServerThread or java PutFileServerThread port");
			System.exit(1);
		}

		startServer();
	}

	public static void startServer() {
		ServerSocket serverSocket = null;
		Socket clientSocket = null;

		try {
			serverSocket = new ServerSocket(port);
			serverSocket.setReuseAddress(true);
			System.out.println("PutFileServerCon: avviato ");
			System.out.println("Server: creata la server socket: " + serverSocket);
		} catch (Exception e) {
			System.err.println("Server: problemi nella creazione della server socket: " + e.getMessage());
			e.printStackTrace();
			System.exit(1);
		}

		try {
			while (true) {
				System.out.print("\nServer: in attesa di richieste...");
				try {
					// bloccante fino ad una pervenuta connessione
					clientSocket = serverSocket.accept();
					clientSocket.setSoTimeout(30000);
					System.out.println("connessione accettata: " + clientSocket);
				} catch (Exception e) {
					System.err.println("problemi nella accettazione della connessione: " + e.getMessage());
					e.printStackTrace();
					continue;
				}

				// serizio delegato ad un nuovo thread
				new PutFileServerThread(clientSocket).start();

			} // while
		} catch (Exception e) {
			e.printStackTrace();
			// chiusura di stream e socket
			System.out.println("PutFileServerCon: termino...");
			System.exit(2);
		}
	}
} // PutFileServerCon class
