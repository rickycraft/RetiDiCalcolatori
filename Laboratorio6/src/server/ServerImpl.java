package server;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
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

	public void main() {

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
}
