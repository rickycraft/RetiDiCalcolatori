package serverSvolta;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

import registry.RegistryRemotoTagServer;

public class ServerCongressoImpl extends UnicastRemoteObject implements ServerCongresso {
	static Programma prog[];

	protected ServerCongressoImpl() throws RemoteException {
		super();
	}

	@Override
	public int registrazione(int giorno, String sessione, String speaker) throws RemoteException {

		int numSess = -1;
		System.out.println("Server RMI: richiesta registrazione:");
		if (sessione.equals("S1"))
			numSess = 0;
		else if (sessione.equals("S2"))
			numSess = 1;
		else if (sessione.equals("S12"))
			numSess = 11;
		/*
		 * Se i dati sono sbagliati significa che sono stati trasmessi male e quindi si
		 * solleva una eccezione
		 */
		if (numSess == -1)
			throw new RemoteException();
		if (giorno < 1 || giorno > 3)
			throw new RemoteException();
		return prog[giorno - 1].registra(numSess, speaker);
	}

	@Override
	public Programma programma(int giorno) throws RemoteException {
		System.out.println("Server RMI: programma giorno " + giorno);
		if (giorno < 1 || giorno > 3)
			throw new RemoteException();
		return prog[giorno - 1];
	}

	public static void main(String[] args) {
		prog = new Programma[3];// creazione dei programmi per le giornate
		for (int i = 0; i < 3; i++)
			prog[i] = new Programma();

		int registryRemotoPort = 1099;
		String registryRemotoServiceName = "RegistryRemoto";
		String myserviceName = "ServerCongresso";
		if (args.length != 1 && args.length != 2) {
			System.err.println("Usage");
			System.exit(1);
		} // Controlloargomenti

		String registryRemotoHost = args[0];
		if (args.length == 2) {
			try {
				registryRemotoPort = Integer.parseInt(args[1]);
			} catch (Exception e) {
				e.printStackTrace();
			}
		} // if

		// Registrazione servizio presso RegistryRemoto
		String completeRemoteRegistryName = "//" + registryRemotoHost + ":" + registryRemotoPort + "/"
				+ registryRemotoServiceName;
		RegistryRemotoTagServer registryRemoto = null;
		try {
			registryRemoto = (RegistryRemotoTagServer) Naming.lookup(completeRemoteRegistryName);
			ServerCongressoImpl serverRMI = new ServerCongressoImpl();
			registryRemoto.aggiungiServer(myserviceName, serverRMI);
		} catch (Exception e) {
			e.printStackTrace();
		}
		BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));

		System.out.println("inserire il tag");
		try {
			registryRemoto.associaTag(registryRemotoServiceName, stdIn.readLine());

		} catch (RemoteException e) {
			System.out.println(e.getMessage());
		} catch (IOException e) {
			System.out.println(e.getMessage());
		}

	}

}
