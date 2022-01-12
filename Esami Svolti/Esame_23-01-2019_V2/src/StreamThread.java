
import java.io.*;
import java.net.*;

public class StreamThread extends Thread {
	private Socket clientSocket=null;

	private String[][] dati;

	public StreamThread(Socket socket, String [][] dati) {
		this.clientSocket = socket;
		this.dati=dati;
	}

	@Override
	public void run() {
		System.out.println("[DEBUG] Attivazione figlio: "+ Thread.currentThread().getName());

		DataInputStream inSock;
		DataOutputStream outSock;

		//CREAZIONE STREAM INPUT E OUTPUT 
		try {
			inSock = new DataInputStream(clientSocket.getInputStream());

			outSock = new DataOutputStream(clientSocket.getOutputStream());
		}
		catch (IOException ioe) {
			System.out.println("[ERROR] Problemi nella creazione degli stream di input/output " );
			ioe.printStackTrace();
			return;
		}

		//CORPO DELLA THREAD
		try {

			while (true) {
				System.out.println("[DEBUG] pre ricezione"); //id tipo luogo data biglietti prezzo

				String[] input=inSock.readUTF().split(" ");
				//Azione COSE
				//System.out.println("[DEBUG]ricevuto richiesta di servizio " + service);

				if (input[0].equals("E")) {
					//SERVIZIO
					//P luogo tipo
					int result=0;
					for(int i=0;i<dati.length;i++) {
						if(dati[i][0].equals(input[1])) {
							dati[i][0]="L";
							dati[i][1]="L";
							dati[i][2]="L";
							dati[i][3]="L";
							result=1;

						}

					}
					outSock.writeInt(result);

					//FINE
				} else if (input[0].equals("S")){
					//SERVIZIO
					//T prezzo
					for(int i=0;i<dati.length;i++) {
						if(dati[i][0].equals(input[1])) {
							String dir=dati[i][3];
							DataInputStream is;
							int buffer;
							long fileSize=0;
							File directory=new File("src/"+dir+"/");
							System.out.println(directory.getAbsolutePath());
							File[] listaFile=directory.listFiles();
						
							//outSock.writeInt(nFile);
							
							for(int j=0;j<listaFile.length;j++) {
								if(listaFile[j].isFile()) {	
									fileSize=listaFile[j].length();
									System.out.println("invio:"+listaFile[j].getName());
									outSock.writeUTF(listaFile[j].getName());
									outSock.writeLong(fileSize);
									is=new DataInputStream(new FileInputStream(listaFile[j]));
									while(fileSize>0) {
										buffer=is.read();
										outSock.write(buffer);
										fileSize--;
										
									}
									System.out.println("fine file invio");
								}		
							}
							outSock.writeUTF("FINE FILE");

						}

					}
					//FINE
				}


			}

			// fine ciclo interazione


		}  catch (EOFException e) {
			System.out.println("Terminazione. (Thread=" + getName() + ")");
		} catch (SocketTimeoutException e) {
			System.out.println("[ERROR]: Socket timed out. (Thread=" + getName() + ")");
			e.printStackTrace();
		} catch (IOException e) {
			System.out.println("[ERROR]: Lettura/Scrittura su stream fallita. (Thread=" + getName() + ")");
			e.printStackTrace();
		} catch (Exception e) {
			System.out.println("[ERROR]: Errore irreversibile. (Thread=" + getName() + ")");
			e.printStackTrace();
		} finally {
			System.out.println("[DEBUG]: Terminazione server thread. (Thread=" + getName() + ")");
			try {
				clientSocket.close();
			} catch (IOException e) {
				System.out.println("[ERROR]: Chiusura socket fallita. (Thread=" + getName() + ")");
				e.printStackTrace();
			}
		}
	}
}
