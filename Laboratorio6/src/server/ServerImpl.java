package server;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class ServerImpl extends UnicastRemoteObject implements RemOp {


	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	/**
	 * 
	 */
	

	protected ServerImpl() throws RemoteException {
		super();
		// TODO Auto-generated constructor stub
	}

	public static void main(String[] args) {
		final int REGISTRYPORT =4500 ;
		String registryHost="localhost";
		/*try {
			registryHost = InetAddress.getLocalHost().getHostAddress();
		} catch (UnknownHostException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}*/
		String serviceName = "RemOp";
		String completeName = "//" + registryHost + ":" + REGISTRYPORT + "/" + serviceName;
		try {
			ServerImpl serverRMI = new ServerImpl();
			Naming.rebind(completeName, serverRMI);
			System.out.println("Server RMI: Servizio \"" + serviceName + "\" registrato");
		} catch (Exception e) {
			System.err.println("Server RMI \"" + serviceName + "\": " + e.getMessage());
			e.printStackTrace();
			System.exit(1);
		}
		
		
	}

	@Override
	public int conta_righe(String nomeFile, int numMin) throws RemoteException{
		// Qui verranno fatti i metodi implementati dall'interfaccia
		int nRighe=0;
		try {
			BufferedReader buff = new BufferedReader(new FileReader(nomeFile));
			String str;

			while((str=buff.readLine())!=null)
			{
				if(str.split(" ").length>numMin)
				{
					nRighe++;
				}
			}

			buff.close();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			throw new RemoteException();

		}

		return nRighe;
	}

	@Override
	public FileRighe elimina_riga(String nomeFile, int num) throws RemoteException {
		// Qui verranno fatti i metodi implementati dall'interfaccia
		System.out.println("nomeFile: "+ nomeFile+" num: "+ num);
		//Considerando che la prima riga sia la numero 1 e non la 0
		//(ha scassato la minchia pure su questo quindi bisogna dirlo all'utente quando lo usa)
		int riga=1;
		int righeNuove=0;

		try {
			BufferedReader buff = new BufferedReader(new FileReader(nomeFile));
			String str;
			File temp=new File(nomeFile+"_temp");
			BufferedWriter buff_temp=new BufferedWriter(new FileWriter(temp));
			while((str=buff.readLine())!=null)
			{

				if(riga!=num)
				{
					buff_temp.write(str+'\n');
					righeNuove++;
				}
				riga++;
			}

			buff.close();
			temp.renameTo(new File(nomeFile));
			buff_temp.close();

		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		if(riga<num){
			//mo so cazzi
			throw new RemoteException();
		}else {
			return new FileRighe(nomeFile,righeNuove);
		}
	}
	public FileRighe elimina_riga2(String nomeFile,int numRigaElim) {
		int indiceRiga=0;
		int righeFile=0;
		
		String lineaLetta;
		try {
			BufferedReader rd=new BufferedReader(new FileReader(nomeFile));
			System.out.println("File aperto");

			File fileTemp=new File(nomeFile+"_temp");
			BufferedWriter wr=new BufferedWriter(new FileWriter(fileTemp));
			System.out.println("FileTenmpCreato");

		
			while((lineaLetta = rd.readLine())!=null) {
				if (indiceRiga!=numRigaElim) {
					wr.write(lineaLetta+"\n");
					System.out.println("riga "+indiceRiga);

					righeFile++;
				}
				indiceRiga++;
			}
			System.out.println(indiceRiga);

			rd.close();
			wr.close();
			if(indiceRiga<=numRigaElim) {
				fileTemp.delete();
				
				throw new RemoteException("non esiste riga numero "+numRigaElim+" nel file "+nomeFile);
			}else {
				File fileOg=new File(nomeFile);
				fileOg.delete();
				

				fileTemp.renameTo(fileOg);
				System.out.println("file sostituito");
				return new FileRighe(nomeFile,righeFile);
			}

		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}	
		catch (IOException e) {
			e.printStackTrace();
		}
		return null;
	}
	
	
	
	
}
