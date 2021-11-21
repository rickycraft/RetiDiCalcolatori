package client;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.rmi.Naming;

import server.RemOp;

public class Client {

	public static void main(String[] args) {

		/*
		 * In pratica il client deve semplicemente poter invocare i metodi di RemOp
		 */

		int registryPort = 1099;
		String registryHost = null; // host remoto con registry
		String serviceName = "RemOp";
		BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));

		// Controllo dei parametri della riga di comando
		if (args.length != 2) {
			System.out.println("Sintassi: RMI_Registry_IP RegistryPort");
			System.exit(1);
		}
		registryHost = args[0];
		registryPort = Integer.parseInt(args[1]);

		System.out.println("Invio richieste a " + registryHost + " per il servizio di nome " + serviceName);

		try {
			String completeName = "//" + registryHost + ":" + registryPort + "/" + serviceName;
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
					Boolean checkIfNum = true;
					while (!checkIfNum) {
						try {
							val = Integer.parseInt(stdIn.readLine());
							checkIfNum = true;
						} catch (NumberFormatException e) {
							System.out.println("scrivi bene il numero per favore\n");
							checkIfNum = false;
						}
					}
					System.out.println("nomeFile: " + nomeFile + "\t num: " + val + "\n");
					int numRighe = serverRMI.conta_righe(nomeFile, val);
					System.out.println("Il numero di righe che hanno piu di " + val + " righe e: " + numRighe + "\n");

				}

				if (azione.equalsIgnoreCase("E")) {
					// chiede elimina_riga

					System.out.println("Inserisci il nome del file:\n");
					String nomeFile = stdIn.readLine();

					System.out.println("Inserisci il numero della riga da eliminare (parte da 1):\n");
					int val = 0;
					Boolean checkIfNum = true;
					while (!checkIfNum) {
						try {
							val = Integer.parseInt(stdIn.readLine());
							checkIfNum = true;
						} catch (NumberFormatException e) {
							System.out.println("scrivi bene il numero per favore\n");
							checkIfNum = false;
						}
					}

					System.out.println("nomeFile: " + nomeFile + "\t num: " + val + "\n");
					String result = serverRMI.elimina_riga(nomeFile, val);
					String[] sep = result.split(";");
					System.out.println("Il file modificato e" + sep[0] + " con " + sep[1] + "righe\n");

				}
				System.out.println("Cosa vuoi fare? E per Elimina_riga, C per conta_righe \n");
			}

		} catch (Exception e) {

		}
	}
}
