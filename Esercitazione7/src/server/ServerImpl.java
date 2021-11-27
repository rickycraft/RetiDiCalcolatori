package server;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class ServerImpl extends UnicastRemoteObject implements RemOp {

	private static final long serialVersionUID = 1L;

	protected ServerImpl() throws RemoteException {
		super();
	}

	public static void main(String[] args) {
		int registryRemotoPort = 1099;
		String registryRemotoName = "RegistryRemoto";
		String serviceName = "RemOp";
		String registryRemotoHost = args[0];
		/*
		 * if (args.length == 1) { try { registryRemotoPort = Integer.parseInt(args[0]);
		 * } catch (NumberFormatException e) {
		 * System.err.println("Formato: ServerImpl [registryPort]\n"); System.exit(-1);
		 * } } else if (args.length > 1) {
		 * System.err.println("Formato: ServerImpl [registryPort]\n"); System.exit(-1);
		 * }
		 */

		String completeRemoteRegistryName = "//" + registryRemotoHost + ":" + registryRemotoPort + "/"
				+ registryRemotoName;
		RegistryRemotoTagServer registryRemoto;
		try {
			registryRemoto = (RegistryRemotoTagServer) Naming.lookup(completeRemoteRegistryName);
			ServerImpl serverRMI = new ServerImpl();
			registryRemoto.aggiungiServer(serviceName, serverRMI);

		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}

	@Override
	public int conta_righe(String nomeFile, int numMin) throws RemoteException {
		// Qui verranno fatti i metodi implementati dall'interfaccia
		int nRighe = 0;
		try {
			BufferedReader buff = new BufferedReader(new FileReader(nomeFile));
			String str;
			while ((str = buff.readLine()) != null) {
				if (str.split(" ").length > numMin) {
					nRighe++;
				}
			}
			buff.close();
		} catch (Exception e) {
			throw new RemoteException(e.getMessage());
		}
		return nRighe;
	}

	@Override
	public String elimina_riga(String nomeFile, int numRigaElim) throws RemoteException {
		int indiceRiga = 0;
		int righeFile = 0;

		String lineaLetta;
		try {
			BufferedReader rd = new BufferedReader(new FileReader(nomeFile));
			System.out.println("File aperto");

			File fileTemp = new File(nomeFile + "_temp");
			BufferedWriter wr = new BufferedWriter(new FileWriter(fileTemp));
			System.out.println("FileTenmpCreato");

			while ((lineaLetta = rd.readLine()) != null) {
				if (indiceRiga != numRigaElim) {
					wr.write(lineaLetta + "\n");
					System.out.println("riga " + indiceRiga);
					righeFile++;
				}
				indiceRiga++;
			}
			System.out.println(indiceRiga);

			rd.close();
			wr.close();
			if (indiceRiga < numRigaElim) {
				fileTemp.delete();
				throw new RemoteException("non esiste riga numero " + numRigaElim + " nel file " + nomeFile);
			} else {
				File fileOg = new File(nomeFile);
				fileOg.delete();

				fileTemp.renameTo(fileOg);
				System.out.println("file sostituito");
				return nomeFile + ";" + righeFile;
			}
		} catch (Exception e) {
			throw new RemoteException(e.getMessage());
		}
	}

}
