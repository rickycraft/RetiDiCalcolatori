package server;

import java.io.DataInputStream;
import java.io.DataOutputStream;
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

		// creazione stream di input e out da socket
		try {
			inSock = new DataInputStream(clientSocket.getInputStream());
			outSock = new DataOutputStream(clientSocket.getOutputStream());
			nomeFile = inSock.readUTF();
		} catch (SocketTimeoutException ste) {
			System.out.println("Timeout scattato: ");
			ste.printStackTrace();
			clientSocket.close();
			System.out.print("\n^D(Unix)/^Z(Win)+invio per uscire, solo invio per continuare: ");
			return;
		} catch (IOException ioe) {
			System.out.println("Problemi nella creazione degli stream di input/output " + "su socket: ");
			ioe.printStackTrace();
			// il server continua l'esecuzione riprendendo dall'inizio del ciclo
			return;
		} catch (Exception e) {
			System.out.println("Problemi nella creazione degli stream di input/output " + "su socket: ");
			e.printStackTrace();
			return;
		}

		// controllo se il file esiste già
		if (nomeFile == null) {
			System.out.println("Problemi nella ricezione del nome del file: ");
			clientSocket.close();
			return;
		} else {
			try {
				esito = (new File(nomeFile).exists()) ? REJECT : ACCEPT;
				outSock.writeUTF(esito); // invio esito a cliente SALTA
			} catch (IOException e) {
				System.out.println("Problemi nella notifica di file esistente: ");
				e.printStackTrace();
				return;
			}
		}

		// se ATTIVA ricevo il file
		if (esito.equals(ACCEPT)) {
			try {
				outFile = new FileOutputStream(nomeFile);
				System.out.println("Ricevo il file " + nomeFile + ": \n");
				FileUtility.trasferisci_a_byte_file_binario(inSock, new DataOutputStream(outFile));
				System.out.println("\nRicezione del file " + nomeFile + " terminata\n");
				// ritorno esito positivo al client
				outSock.writeUTF(esito + ", file salvato lato server");
				System.out.println("\nTerminata connessione con " + clientSocket);
			} catch (SocketTimeoutException ste) {
				System.out.println("Timeout scattato: ");
				ste.printStackTrace();
				System.out.print("\n^D(Unix)/^Z(Win)+invio per uscire, solo invio per continuare: ");
				return;
			} catch (Exception e) {
				System.err.println("\nProblemi durante la ricezione e scrittura del file: " + e.getMessage());
				e.printStackTrace();
				System.out.println("Terminata connessione con " + clientSocket);
				return;
			} finally {
				outFile.close(); // chiusura file
				clientSocket.shutdownInput(); // chiusura socket (downstream)
				clientSocket.shutdownOutput(); // chiusura socket (upstream)
				clientSocket.close();
			}
		}
		// salta non faccio niente
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