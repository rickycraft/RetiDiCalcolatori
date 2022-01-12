
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.rmi.Naming;
import java.rmi.RemoteException;

//import .RMI_interfaceFile;

public class RMI_Client {

	private static final String cosavuoifare = "Cosa vuoi fare? A per Acquistare biglietti, I per inserire degli eventi, qualsiasi altro carattere per uscire";

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

				if (azione.equalsIgnoreCase("A")) {
					// Chiede conta_righe
					System.out.println("Inserisci il nome dell'evento:");
					String id= stdIn.readLine();
					System.out.println("Inserisci il numero di biglietti che vuoi acquistare:");

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

					System.out.println("id: " + id + "\t num: " + val);
					try {
						int result = serverRMI.acquista_biglietti(id, val);
						if(result>0) {
							System.out.println("L'acquisto dei biglietti è avvenuto con successo");
						}else {
							System.out.println("C'è stato un errore durante l'acquisto");
						}

					} catch (RemoteException re) {
						System.out.println(re.getMessage());
					}
				} else if (azione.equalsIgnoreCase("I")) {
					// chiede elimina_riga
					String id="";
					boolean corretto=false;
					while(!corretto) {
						System.out.println("Inserisci il nome dell'evento:");
						id= stdIn.readLine();
						if(!id.equals("L")) {
							corretto=true;
						}else {
							System.out.println("Il nome dell'evento non può essere L\n");
						}
					}
					System.out.println("Inserisci il tipo di evento:");
					String tipo="";
					corretto=false;
					while(!corretto) {
						tipo=stdIn.readLine();
						if(tipo.equals("Concerto")||tipo.equals("Calcio")||tipo.equals("Formula1")) {
							corretto=true;
						}
					}

					System.out.println("Inserisci il luogo dell'evento:");
					String luogo= stdIn.readLine();

					System.out.println("Inserisci la data nel formato gg/mm/aaaa:");
					String data="";
					corretto=false;
					while(!corretto) {
						data=stdIn.readLine();

						String[] controlloData=data.split("/");
						try {
							Integer.parseInt(controlloData[0]);
							Integer.parseInt(controlloData[1]);
							Integer.parseInt(controlloData[2]);

						}catch(NumberFormatException e) {
							System.out.println("Devi inserire la data correttamente");
							}finally {

							if(controlloData.length==3) {
								if(controlloData[0].length()==2) {
									if(controlloData[1].length()==2) {
										if(controlloData[2].length()==4) {
											corretto=true;
										}
									}
								}
							}
						}
					}


					System.out.println("Inserisci il numero di biglietti:");
					int numBiglietti = 0;
					boolean checkIfNum = false;
					while (!checkIfNum) {
						try {
							numBiglietti= Integer.parseInt(stdIn.readLine());
							checkIfNum = true;
						} catch (NumberFormatException e) {
							System.out.println("scrivi bene il numero per favore");
							checkIfNum = false;
						}
					}
					
					System.out.println("Inserisci il prezzo (intero)\n:");
					int prezzo= 0;
					checkIfNum = false;
					while (!checkIfNum) {
						try {
							prezzo= Integer.parseInt(stdIn.readLine());
							checkIfNum = true;
						} catch (NumberFormatException e) {
							System.out.println("scrivi bene il numero per favore");
							checkIfNum = false;
						}
					}

					System.out.println("id: " + id+ "\t tipo: " + tipo+ "\t luogo: "+luogo+"\t data: "+data+"\t numBiglietti: "+numBiglietti+"\t prezzo:" +prezzo+"\n");
					try {
						int result = serverRMI.inserimento_evento(id, tipo, luogo, data, numBiglietti, prezzo);
						if(result>0) {
							System.out.println("L'aggiunta dell'evento è avvenuta con successo");
						}else {
							System.out.println("C'è stato un errore durante l'aggiunta");
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
