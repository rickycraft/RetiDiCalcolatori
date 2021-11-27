package client;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.rmi.Naming;

import registry.RegistryRemotoTagClient;
import serverSvolta.Programma;
import serverSvolta.ServerCongresso;

public class ClientCongresso {
	public static void main(String[] args) {
		int registryRemotoPort = 1099;
		String registryRemotoName = "RegistryRemoto";
		String targetServiceName = "ServerCongresso";
		BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));

		try {
			// Controllo dei parametri della riga di comando
			if (args.length != 1 && args.length != 2) {
				System.out.println("Sintassi:...");
				System.exit(1);
			}
			String registryRemotoHost = args[0];
			if (args.length == 2) {
				try {
					registryRemotoPort = Integer.parseInt(args[1]);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
			// Connessione al servizio RMI remoto
			String completeRemoteRegistryName = "//" + registryRemotoHost + ":" + registryRemotoPort + "/"
					+ registryRemotoName;
			RegistryRemotoTagClient registryRemoto = (RegistryRemotoTagClient) Naming
					.lookup(completeRemoteRegistryName);
			ServerCongresso serverRMI = null;

			boolean trovato = false;
			System.out.println("vuoi cercare per nome logico (n) o per tag(t)");
			String letta = stdIn.readLine();
			while (!trovato) {
				if (letta.equals("n")) {
					serverRMI = (ServerCongresso) registryRemoto.cerca(targetServiceName);
					trovato = true;
				} else {
					if (letta.equals("t")) {
						System.out.println("inserire il tag");
						String[] serverNames = registryRemoto.cercaTag(stdIn.readLine());
						for (String serverName : serverNames) {
							System.out.println(serverName);
						}
					}
				}
				System.out.println("vuoi cercare per nome logico (n) o per tag(t)");
				letta = stdIn.readLine();

			}

			System.out.println("\nRichieste a EOF");
			System.out.print("Servizio(R=Registrazione, P=Programma): ");
			String service;
			boolean ok;
			// Ciclo di interazione con l’utente per chiedere operazioni
			while ((service = stdIn.readLine()) != null) {
				if (service.equals("R")) {
					ok = false;
					int g = 0;// letturagiornata
					System.out.print("Giornata(1-3)? ");
					while (ok != true) {
						g = Integer.parseInt(stdIn.readLine());
						if (g < 1 || g > 3) {
							System.out.println("Giornata non valida");
							System.out.print("Giornata(1-3)? ");
							continue;
						} else
							ok = true;
					} // end while giornata
					ok = false;
					String sess = "S1"; // letturasessione
					System.out.print("Sessione(S1 -S12)? ");
					while (ok != true) {
						sess = stdIn.readLine();
						if (!sess.equals("S1") && !sess.equals("S2") && !sess.equals("S12")) {
							System.out.print("Sessione(S1 -S12)? ");
							continue;

						} else
							ok = true;
					} // end while sessione

					System.out.print("Speaker? "); // letturaspeaker
					String speak = stdIn.readLine();// Parametri corretti, invoco il servizio remoto
					if (serverRMI.registrazione(g, sess, speak) == 0)
						System.out.println("Registrazione di ...");
					else
						System.out.println("Registrazione non effettuata");
				} // end if service R
				else if (service.equals("P")) {
					int g = 0;
					ok = false;
					System.out.print("Giornata(1-3)? ");
					while (ok != true) {
						g = Integer.parseInt(stdIn.readLine());
						if (g < 1 || g > 3) {
							System.out.println("Giornata non valida");
							System.out.print("Giornata(1-3)? ");
							continue;
						} else
							ok = true;
					} // while
					Programma prog = serverRMI.programma(g);
					System.out.println("Programma giornata " + g + "\n");
					prog.stampa();
				} // end if service P
				else
					System.out.println("Servizio non disponibile");
				System.out.print("Servizio(R=Registrazione, P=Programma): ");

			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
