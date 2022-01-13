
import java.io.*;
import java.net.*;
import java.nio.file.Path;
import java.nio.file.Paths;

public class StreamThread extends Thread {
	private Socket clientSocket=null;



	public StreamThread(Socket socket) {
		this.clientSocket = socket;
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

				String selezione=inSock.readUTF();
				//Azione COSE
				//System.out.println("[DEBUG]ricevuto richiesta di servizio " + service);

				if (selezione.equals("E")) { //elimina occorrenze da un file.
					//SERVIZIO
					//P luogo tipo
					int result=-1;
					String nomeFile=inSock.readUTF();
					String parola=inSock.readUTF();
					String riga;
					File dirCorrente=new File("").getAbsoluteFile();
					System.out.println(dirCorrente.getPath());
					File[] listaFile=dirCorrente.listFiles();
					File fileTemp=new File(nomeFile+"_temp");
					BufferedReader br;
					BufferedWriter bw;

					for(int i=0;i<listaFile.length;i++) {

						try{if(listaFile[i].isFile()) {
							if(listaFile[i].getName().equals(nomeFile)) {
								result=0;
								System.out.println(listaFile[i].getPath());
								br=new BufferedReader(new FileReader(listaFile[i]));
								bw=new BufferedWriter(new FileWriter(fileTemp));
								while((riga=br.readLine())!=null) {
									String[] parolePerRiga=riga.split(" ");
									for(int j=0;j<parolePerRiga.length;j++) {
										if(parolePerRiga[j].equals(parola)) {
											result++;
											if(j+1==parolePerRiga.length) {
												bw.write("\n");
											}
										}else {
											bw.write(parolePerRiga[j]);
											if(j+1==parolePerRiga.length) {
												bw.write("\n");
											}else {
												bw.write(" ");
											}
										}
									}		
								}
								listaFile[i].delete();
								if(fileTemp.renameTo(new File(listaFile[i].getPath()))) {
									System.out.println("Il file è stato rinominato");
								}else {

									System.out.println("Il file non è stato rinominato");
								}

								break;
							}
						}
						}catch(Exception e) {

						}

					}
					outSock.writeInt(result);

					//FINE
				} else if (selezione.equals("S")){//trasferimento tutti file binari di un direttorio
					//SERVIZIO
					//T prezzo
					String direttorio=inSock.readUTF();
					int sogliaMinima=inSock.readInt();
					DataInputStream is;
					int buffer;
					long fileSize=0;
					File directory=new File(direttorio);
					System.out.println(directory.getAbsolutePath());
					File[] listaFile=directory.listFiles();

					//outSock.writeInt(nFile);

					for(int j=0;j<listaFile.length;j++) {
						if(listaFile[j].isFile()) {	

							if((fileSize=listaFile[j].length())>sogliaMinima) {
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

					}
					outSock.writeUTF("FINE FILE");

					//FINE



				}

				// fine ciclo interazione

			}
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
