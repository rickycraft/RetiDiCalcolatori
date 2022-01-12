import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class RMI_Server extends UnicastRemoteObject implements RMI_interfaceFile{

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	public static final int MAX_EVENTI=10;
	private Eventi[] eventi;
	protected RMI_Server() throws RemoteException {
		super();
		eventi=new Eventi[MAX_EVENTI];
		formattaEventi();
	}

	public static void main(String[] args) {
		int registryPort = 4500;
		String registryHost = "localhost";
		if (args.length == 1) {
			try {
				registryPort = Integer.parseInt(args[0]);
			} catch (NumberFormatException e) {
				System.err.println("Formato: ServerImpl [registryPort]\n");
				System.exit(-1);
			}
		} else if (args.length > 1) {
			System.err.println("Formato: ServerImpl [registryPort]\n");
			System.exit(-1);
		}

		String serviceName = "RMI_interfaceFile";
		String completeName = "//" + registryHost + ":" + registryPort + "/" + serviceName;
		try {
			RMI_Server serverRMI = new RMI_Server();
			Naming.rebind(completeName, serverRMI);
			System.out.println("Server RMI: Servizio \"" + serviceName + "\" registrato");
		} catch (Exception e) {
			System.err.println("Server RMI \"" + serviceName + "\": " + e.getMessage());
			e.printStackTrace();
			System.exit(1);
		}

	}
	
	@Override
	public synchronized int inserimento_evento(String id, String tipo, String luogo, String data, int nBiglietti, int prezzo)
			throws RemoteException {
		//aggiungo evento
		for(int i=0;i<MAX_EVENTI;i++) {
			if(this.eventi[i].getId().equals("L")) {
				this.eventi[i].setId(id);
				this.eventi[i].setTipo(tipo);
				this.eventi[i].setLuogo(luogo);
				this.eventi[i].setData(data);
				this.eventi[i].setNumBiglietti(nBiglietti);
				this.eventi[i].setPrezzo(prezzo);
				return 1;
			}
		}
		// TODO Auto-generated method stub
		return -1;
	}

	@Override
	public synchronized int acquista_biglietti(String id, int numBiglietti) throws RemoteException {

		for(int i=0;i<MAX_EVENTI;i++) {
			if(this.eventi[i].getId().equals(id)) {
			
				if(this.eventi[i].getNumBiglietti()>=numBiglietti) {
					this.eventi[i].setNumBiglietti(this.eventi[i].getNumBiglietti()-numBiglietti);
					return 1;
				}
			}
			
			
		}
		return -1;
	}
	
	private void formattaEventi() {
		for(int i=0;i<MAX_EVENTI;i++) {
			this.eventi[i]=new Eventi("L","L","L","L",-1,-1);
		}
	}

}
