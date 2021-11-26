package server;

import java.rmi.Remote;
import java.rmi.RemoteException;

public class RegistryRemotoTagImpl implements RegistryRemotoTagServer{

	//Implementando RegistryRemotoTagServer implemento tutto perchè un'interfaccia estende le altre che a sua volta estendono altre ancora

	private int grandezza=100;


	private Object[][] map=new Object[grandezza][3];
	//3 per nome_logico, rif_remoto e tag

	//Formato nel seguente modo:
	// NOME_LOGICO, REMOTE , TAG


	public RegistryRemotoTagImpl() {
		super();
		for(int i=0;i<grandezza;i++) {
			for(int j=0;j<3;j++) {
				map[i][j]=null;
			}
		}
	}

	@Override
	public boolean aggiungiServer(String nome_logico, Remote rif_remoto) throws RemoteException {
		if(nome_logico.isEmpty() || rif_remoto ==null)
			throw new RemoteException();
		else {
			for(int i=0;i<grandezza;i++) {
				if(map[i][0]==null) {
					map[i][0]=nome_logico;
					map[i][1]=rif_remoto;
					return true;
				}
			}

		}


		return false;
	}

	@Override
	public boolean eliminaPrimo(String nome_logico) throws RemoteException {

		if(nome_logico.isEmpty())
			throw new RemoteException();
		else {
			for(int i=0;i<grandezza;i++) {
				if(map[i][0].equals(nome_logico)) {
					map[i][0]=null;
					map[i][1]=null;
					map[i][2]=null;
					return true;
				}
			}

		}
		return false;
	}

	@Override
	public Remote cerca(String nomeLogico) throws RemoteException {

		if(nomeLogico.isEmpty())
			throw new RemoteException();
		else {
			for(int i=0;i<grandezza;i++) {
				if(map[i][0].equals(nomeLogico)) {
					return (Remote)map[i][1];
				}
			}

		}

		return null;
	}

	@Override
	public Remote[] cercaTutti(String nomeLogico) throws RemoteException {
		//Qua secondo me sarebbe meglio fare in modo che magari faccia un primo ciclo per vedere quanti remote ci sono e poi fare un array
		//della grandezza giusta ma potrebbe cagare il cazzo perchè farebbe due cicli
		if(nomeLogico.isEmpty())
			throw new RemoteException();
		else {



			int j=0;
			for(int i=0;i<grandezza;i++) {
				if(map[i][0].equals(nomeLogico)) {
					j++;
				}
			}
			if(j==0)
				return null;

			Remote[] result=new Remote[j];
			j=0;
			for(int i=0;i<grandezza;i++) {
				if(map[i][0].equals(nomeLogico)) {
					result[j]=(Remote) map[i][1];
					j++;
				}
			}
			return result;
		}
		
	}

	@Override
	public String[] cercaTag(String tag) throws RemoteException {
		//Anche qui vale lo stesso commento di cercaTutti

		if(tag.isEmpty())
			throw new RemoteException();
		else {
			int j=0;
			for(int i=0;i<grandezza;i++) {
				if(map[i][2].equals(tag)) {
					j++;
				}
			}
			if(j==0)
				return null;
			String[] result=new String[j];
			j=0;
			for(int i=0;i<grandezza;i++) {
				if(map[i][2].equals(tag)) {
					result[j]=(String) map[i][2];
					j++;
				}
			}
			return result;
		}
	}

	@Override
	public int associaTag(String nome_logico_server, String tag) throws RemoteException {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public Object[][] restituisciTutti() throws RemoteException {
		int count=0;
		for(int i=0;i<this.grandezza;i++) {
			if(map[i][0]!=null) {
				count++;
			}
		}
		if(count==0)
			throw new RemoteException();

		else {
			Object[][] result=new Object[count][3];

			count=0;
			for(int i=0;i<this.grandezza;i++) {
				if(map[i][0]!=null) {

					result[count][0]=map[i][0];
					result[count][1]=map[i][1];
					result[count][2]=map[i][2];
					count++;
				}
			}

			return result;

		}

	}

	@Override
	public boolean eliminaTutti(String nome_logico) throws RemoteException {
		int numEliminati=0;
		if(nome_logico.isEmpty())
			throw new RemoteException();
		else {
			for(int i=0;i<grandezza;i++) {
				if(map[i][0].equals(nome_logico)) {
					map[i][0]=null;
					map[i][1]=null;
					map[i][2]=null;
					numEliminati++;
				}
			}

		}

		if(numEliminati!=0)
			return true;
		return false;
	}

}
