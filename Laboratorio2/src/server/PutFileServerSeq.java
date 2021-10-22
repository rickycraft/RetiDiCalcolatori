// PutFileServer Sequenziale
package server;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.EOFException;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketTimeoutException;

import utility.FileUtility;

public class PutFileServerSeq {
	public static int port = 54321; // porta default per server
	public static String ACCEPT = "attiva";
	public static String REJECT = "salta file";

	public static void main(String[] args) throws IOException {

		/* controllo argomenti */
		try {
			if (args.length == 1) {
				port = Integer.parseInt(args[0]);
				// controllo che la porta sia nel range consentito 1024-65535
				if (port < 1024 || port > 65535) {
					System.out.println("Usage: java PutFileServerSeq or java PutFileServerSeq port");
					System.exit(1);
				}
			} else if (args.length > 1) {
				System.out.println("Usage: java PutFileServerSeq or java PutFileServerSeq port");
				System.exit(1);
			}
		} // try
		catch (Exception e) {
			System.out.println("Problemi, i seguenti: ");
			e.printStackTrace();
			System.out.println("Usage: java PutFileServerSeq or java PutFileServerSeq port");
			System.exit(1);
		}

		startServer();
	}

	public static void startServer() {
		/* preparazione socket e in/out stream */
		ServerSocket serverSocket = null;
		try {
			serverSocket = new ServerSocket(port);
			serverSocket.setReuseAddress(true);
			System.out.println("PutFileServerSeq: avviato ");
			System.out.println("Creata la server socket: " + serverSocket);
		} catch (Exception e) {
			System.err.println("Problemi nella creazione della server socket: " + e.getMessage());
			e.printStackTrace();
			System.exit(2);
		}
		try {
			Socket clientSocket = null;
			DataInputStream inSock = null;
			DataOutputStream outSock = null;
			String nomeFile;
			FileOutputStream outFile = null;
			String esito;
			File f;

			// ciclo infinito server
			while (true) {
				System.out.println("\nIn attesa di richieste...");
				try {
					clientSocket = serverSocket.accept();
					clientSocket.setSoTimeout(30000); // timeout altrimenti server sequenziale si sospende
					System.out.println("Connessione accettata: " + clientSocket + "\n");
					inSock = new DataInputStream(clientSocket.getInputStream());
					outSock = new DataOutputStream(clientSocket.getOutputStream());
				} catch (SocketTimeoutException te) {
					System.err.println("Non ho ricevuto nulla dal client per 30 sec");
					continue;
				} catch (Exception e) {
					System.err.println("Problemi nella accettazione della connessione: " + e.getMessage());
					e.printStackTrace();
					continue;
				}
				while (clientSocket.isConnected()) {
					// ricezione nome file
					try {
						nomeFile = inSock.readUTF();
					} catch (EOFException e) {
						System.out.println("Socket chiusa dal client");
						clientSocket.shutdownInput();
						clientSocket.shutdownOutput();
						clientSocket.close();
						break;
					}

					// controllo se il file esiste già
					if (nomeFile == null) {
						System.out.println("Problemi nella ricezione del nome del file: ");
						clientSocket.close();
						continue;
					} else {
						f = new File(nomeFile);
						esito = (f.exists()) ? REJECT : ACCEPT;
						try {
							outSock.writeUTF(esito); // invio esito a cliente SALTA
						} catch (Exception e) {
							System.out.println("Problemi nella notifica di file esistente: ");
							e.printStackTrace();
							continue;
						}
					}

					// se ATTIVA ricevo il file
					if (esito.equals(ACCEPT)) {
						try {
							long fileDim = inSock.readLong();
							outFile = new FileOutputStream(nomeFile);
							System.out.printf("Inizio ricezione <%s>...", nomeFile);
							FileUtility.trasferisci_a_byte_file_binario(inSock, new DataOutputStream(outFile), fileDim);
							System.out.println("terminata");
						} catch (SocketTimeoutException ste) {
							System.out.println("Timeout scattato: ");
							ste.printStackTrace();
							System.out.print("\n^D(Unix)/^Z(Win)+invio per uscire, solo invio per continuare: ");
							continue;
						} catch (Exception e) {
							System.err
									.println("\nProblemi durante la ricezione e scrittura del file: " + e.getMessage());
							e.printStackTrace();
							System.out.println("Terminata connessione con " + clientSocket);
							continue;
						} finally {
							outFile.close(); // chiusura file
						}
					} // if (esito.equals(ACCEPT))
				} // while (socket.isConnected)
			} // while (true)

		}
		// qui catturo le eccezioni non catturate all'interno del while
		// in seguito alle quali il server termina l'esecuzione
		catch (Exception e) {
			e.printStackTrace();
			// chiusura di stream e socket
			System.out.println("Errore irreversibile, PutFileServerSeq: termino...");
			System.exit(3);
		}
	} // main
} // PutFileServerSeq
