package server;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.EOFException;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.net.SocketTimeoutException;

import utility.FileUtility;

class PutFileServerThread extends Thread {

	private Socket clientSocket = null;
	public String ACCEPT = "attiva";
	public String REJECT = "salta file";

	/**
	 * Constructor
	 * 
	 * @param clientSocket
	 */
	public PutFileServerThread(Socket clientSocket) {
		this.clientSocket = clientSocket;
	}

	public void wrapper() throws IOException {
		String nomeFile = null;
		DataInputStream inSock;
		DataOutputStream outSock;
		FileOutputStream outFile = null;
		String esito;
		File f;

		// creazione stream di input e out da socket
		try {
			inSock = new DataInputStream(clientSocket.getInputStream());
			outSock = new DataOutputStream(clientSocket.getOutputStream());
		} catch (Exception e) {
			System.out.println("Problemi nella creazione degli stream di input/output " + "su socket: ");
			e.printStackTrace();
			return;
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
					System.err.println("\nProblemi durante la ricezione e scrittura del file: " + e.getMessage());
					e.printStackTrace();
					System.out.println("Terminata connessione con " + clientSocket);
					continue;
				} finally {
					outFile.close(); // chiusura file
				}
			} // if (esito.equals(ACCEPT))
		}
	}

	@Override
	public void run() {
		try {
			wrapper();
		} catch (Exception e) {
			e.printStackTrace();
			System.out.println("Errore irreversibile, PutFileServerThread: termino...");
			System.exit(3);
		}
	} // run

} // PutFileServerThread class