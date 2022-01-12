
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.rmi.Naming;
import java.rmi.RemoteException;

//import .RMI_interfaceFile;

public class RMI_Client {

	private static final String cosavuoifare = "Cosa vuoi fare? E per eliminare una prenotazione, V per visualizzare le prenotazioni di un certo tipo, qualsiasi altro carattere per uscire";

	public static void main(String[] args) {

		/*
		 * In pratica il client deve semplicemente poter invocare i metodi di RemOp
		 */

		int registryPort = 4500;
		String registryHost = null; // host remoto con registry
		String serviceName = "RMI_interfaceFile";
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
			RMI_interfaceFile serverRMI = (RMI_interfaceFile) Naming.lookup(completeName);
			System.out.println("ClientRMI: Servizio \"" + serviceName + "\" connesso");

			System.out.println(cosavuoifare);
			String azione;
			while ((azione = stdIn.readLine()) != null) {

				if (azione.equalsIgnoreCase("E")) {
					// Chiede conta_righe
					System.out.println("Inserisci la targa del veicolo:");
					String targa= stdIn.readLine();
										try {
						int result = serverRMI.elimina_prenotazione(targa);
						if(result>0) {
							System.out.println("Operazione avvenuta con successo");
						}else {
							System.out.println("Operazione non riuscita");
						}

					} catch (RemoteException re) {
						System.out.println(re.getMessage());
					}
				} else if (azione.equalsIgnoreCase("V")) {
					// chiede elimina_riga
					String tipo="";
					boolean corretto=false;
					while(!corretto) {
						System.out.println("Inserisci tipo di veicolo:");
						tipo= stdIn.readLine();
						if(tipo.equalsIgnoreCase("auto")||tipo.equalsIgnoreCase("camper")) {
							corretto=true;
						}else {
							System.out.println("Puoi scegliere solo tra auto e camperL\n");
						}
					}
					
					System.out.println("tipo: " + tipo);
					try {
						System.out.println("invio richiesta\n");
						Prenotazione[] result = serverRMI.visualizza_prenotazioni(tipo);
						System.out.println("ricevo risposta\n");
						if(result==null) {
							System.out.println("L'Operazione di visualizzazione non è riuscita");
						}else {
							System.out.println("Targa\tTipoVeicolo\tPatente\tDir_Immagini\n");
							for(int i=0;i<result.length;i++) {
								System.out.println(result[i].getTarga()+"\t"+result[i].getTipoVeicolo()+"\t"+result[i].getPatente()+"\t"+result[i].getDir_immagini()+"\n");
							}
						}
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
