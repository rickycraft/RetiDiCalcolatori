package client;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.rmi.Naming;
import java.rmi.RemoteException;

import server.RegistryRemotoTagClient;
import server.RemOp;

public class ClientServerRighe {

	private static final String cosavuoifare = "Cosa vuoi fare? E per Elimina_riga, C per conta_righe, qualsiasi altro carattere per uscire";

	public static void main(String[] args) {

		/*
		 * In pratica il client deve semplicemente poter invocare i metodi di RemOp
		 */

		int registryRemotoPort = 1099;
		String registryRemotoName = "RegistryRemoto"; // host remoto con registry
		String serviceName = "RemOp";
		BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));

		String registryRemotoHost = args[0];

		String completeRemoteRegistryName = "//" + registryRemotoHost + ":" + registryRemotoPort + "/"
				+ registryRemotoName;

		RegistryRemotoTagClient registryRemoto;
		try {
			registryRemoto = (RegistryRemotoTagClient) Naming.lookup(completeRemoteRegistryName);
			RemOp serverRMI = (RemOp) registryRemoto.cerca(serviceName);

			// codice normale
			System.out.println("ClientRMI: Servizio \"" + serviceName + "\" connesso");

			System.out.println(cosavuoifare);
			String azione;
			while ((azione = stdIn.readLine()) != null) {

				if (azione.equalsIgnoreCase("C")) {
					// Chiede conta_righe
					System.out.println("Inserisci il nome del file:");
					String nomeFile = stdIn.readLine();

					System.out.println("Inserisci il numero:");
					int val = 0;
					Boolean checkIfNum = false;
					while (!checkIfNum) {
						try {
							val = Integer.parseInt(stdIn.readLine());
							checkIfNum = true;
						} catch (NumberFormatException e) {
							System.out.println("scrivi bene il numero per favore");
							checkIfNum = false;
						}
					}
					System.out.println("nomeFile: " + nomeFile + "\t num: " + val);

					try {
						int numRighe = serverRMI.conta_righe(nomeFile, val);
						System.out.println("Il numero di righe che hanno piu di " + val + " righe e: " + numRighe);
					} catch (RemoteException re) {
						System.out.println(re.getMessage());
					}
				} else if (azione.equalsIgnoreCase("E")) {
					// chiede elimina_riga
					System.out.println("Inserisci il nome del file:");
					String nomeFile = stdIn.readLine();
					System.out.println("Inserisci il numero della riga da eliminare (parte da 0):");
					int val = 0;
					boolean checkIfNum = false;
					while (!checkIfNum) {
						try {
							val = Integer.parseInt(stdIn.readLine());
							checkIfNum = true;
						} catch (NumberFormatException e) {
							System.out.println("scrivi bene il numero per favore");
							checkIfNum = false;
						}
					}

					System.out.println("nomeFile: " + nomeFile + "\t num: " + val + "");
					try {
						String result = serverRMI.elimina_riga(nomeFile, val);
						String[] sep = result.split(";");
						System.out.println("Il file modificato e " + sep[0] + " con " + sep[1] + " righe");

					} catch (RemoteException re) {
						System.out.println(re.getMessage());
					}
				} else {
					System.out.println("Esco come richiesto");
					System.exit(0);
				}
				System.out.println(cosavuoifare);
			}
		} catch (Exception e) {
			System.out.println("Eccezione generica " + e.getMessage());
		}
	}
}