package client;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.rmi.Naming;

import server.FileRighe;
import server.RemOp;

public class Client {

	public static void main(String[] args) {

		/*
		 * In pratica il client deve semplicemente poter invocare i metodi di RemOp
		 */

		final int REGISTRYPORT = 1099;
		String registryHost = null; // host remoto con registry
		String serviceName = "";
		BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));

		// Controllo dei parametri della riga di comando
		if (args.length != 2) {
			System.out.println("Sintassi: RMI_Registry_IP ServiceName");
			System.exit(1);
		}
		registryHost = args[0];
		serviceName = args[1];

		System.out.println("Invio richieste a " + registryHost + " per il servizio di nome " + serviceName);

		try {
			String completeName = "//" + registryHost + ":" + REGISTRYPORT + "/" + serviceName;
			RemOp serverRMI = (RemOp) Naming.lookup(completeName);
			System.out.println("ClientRMI: Servizio \"" + serviceName + "\" connesso\n");

			System.out.println("Cosa vuoi fare? E per Elimina_riga, C per conta_righe ");
			String azione;
			while ((azione = stdIn.readLine()) != null) {

				if (azione.equalsIgnoreCase("C")) {
					// Chiede conta_righe
					System.out.println("Inserisci il nome del file:\n");
					String nomeFile = stdIn.readLine();

					System.out.println("Inserisci il numero:\n");
					int val = 0;
					Boolean bool = true;
					try {
						val = Integer.parseInt(stdIn.readLine());
					} catch (NumberFormatException e) {
						System.out.println("scrivi bene il numero\n");
						bool = false;
					}
					if (bool) {
						System.out.println("nomeFile: " + nomeFile + "\t num: " + val + "\n");
						int numRighe = serverRMI.conta_righe(nomeFile, val);
						System.out.println(
								"Il numero di righe che hanno più di " + val + " righe è: " + numRighe + "\n");
					}


				}

				if (azione.equalsIgnoreCase("E")) {
					// chiede elimina_riga

					System.out.println("Inserisci il nome del file:\n");
					String nomeFile = stdIn.readLine();

					System.out.println("Inserisci il numero della riga da eliminare (parte da 1):\n");
					int val = 0;
					Boolean valCorretto = false;
					while (!valCorretto) {
						try {
							val = Integer.parseInt(stdIn.readLine());
							valCorretto = true;
						} catch (NumberFormatException e) {
							System.out.println("scrivi bene il numero\n");
						}
					}

					System.out.println("nomeFile: " + nomeFile + "\t num: " + val + "\n");
					FileRighe result= serverRMI.elimina_riga(nomeFile, val);
					System.out.println("Il file modificato è" + result.getNomeFile() + " con " + result.getnRighe() + "righe\n");



				}
				System.out.println("Cosa vuoi fare? E per Elimina_riga, C per conta_righe \n");
			}

		} catch (Exception e) {

		}
	}
}
