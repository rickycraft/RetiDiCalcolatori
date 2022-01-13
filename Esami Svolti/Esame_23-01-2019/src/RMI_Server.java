import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class RMI_Server extends UnicastRemoteObject implements RMI_interfaceFile{

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	public static final int MAX_EVENTI=10;
	private Prenotazione[] prenotazioni;
	protected RMI_Server() throws RemoteException {
		super();
		prenotazioni=new Prenotazione[MAX_EVENTI];
		formattaPrenotazioni();
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
	public synchronized int elimina_prenotazione(String targa)
			throws RemoteException {
		//aggiungo evento
		for(int i=0;i<MAX_EVENTI;i++) {
			if(this.prenotazioni[i].getTarga().equals(targa)) {
				this.prenotazioni[i].svuota();
				return 1;
			}
		}
		// TODO Auto-generated method stub
		return -1;
	}

	@Override
	public Prenotazione[] visualizza_prenotazioni(String tipoVeicolo) throws RemoteException {
		Prenotazione[] result=null;
		int grandezzaArray=0;
		for(int i=0;i<MAX_EVENTI;i++) {
			if(!this.prenotazioni[i].getTarga().equals("L")) {
				if(this.prenotazioni[i].getTarga().substring(0, 2).compareToIgnoreCase("ED")>0) {
					if(this.prenotazioni[i].getTipoVeicolo().equals(tipoVeicolo)) {
						grandezzaArray++;
					}
				}
			}
		}
		System.out.println("grandezzaArray:"+grandezzaArray+"\n");
		if(grandezzaArray!=0) {
			int indice=0;
			result=new Prenotazione[grandezzaArray];
			for(int i=0;i<MAX_EVENTI;i++) {
				if(!this.prenotazioni[i].getTarga().equals("L")) {
					if(this.prenotazioni[i].getTarga().substring(0, 2).compareToIgnoreCase("ED")>0) {
						if(this.prenotazioni[i].getTipoVeicolo().equals(tipoVeicolo)) {
							result[indice]=new Prenotazione();
							result[indice].setPrenotazione(this.prenotazioni[i].getTarga(),this.prenotazioni[i].getPatente(), 
									tipoVeicolo, this.prenotazioni[i].getDir_immagini());
							indice++;
						}
					}
				}
			}
			System.out.println("Targa\tTipoVeicolo\tPatente\tDir_Immagini\n");
			for(int i=0;i<result.length;i++) {
				System.out.println(result[i].getTarga()+"\t"+result[i].getTipoVeicolo()+"\t"+result[i].getPatente()+"\t"+result[i].getDir_immagini()+"\n");
			}
		}
		
		
		return result;
	}

	private void formattaPrenotazioni() {
		for(int i=0;i<MAX_EVENTI;i++) {
			this.prenotazioni[i]=new Prenotazione();
		}

		this.prenotazioni[0].setPrenotazione("EF123AB", "12355","auto", "EF123AB_img");

		this.prenotazioni[1].setPrenotazione("FF321AB", "12355","auto", "EF123AB_img");

		this.prenotazioni[2].setPrenotazione("ZZ123AB", "12355","camper", "EF123AB_img");
		this.prenotazioni[3].setPrenotazione("XX123AB", "12355","camper", "EF123AB_img");
	}

}
