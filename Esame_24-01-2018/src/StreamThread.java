
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
                
                if (selezione.equals("C")) { //contare numero d linee con n occorrenze di un carattere inviato
                //SERVIZIO
                	//P luogo tipo
                	int result=0;
                	int numOccorrenze=inSock.readInt();
                	char carattere=inSock.readChar();
                	
                	File dirCorrente=new File("").getAbsoluteFile();
                	System.out.println(dirCorrente.getPath());
					File[] listaFile=dirCorrente.listFiles();
					BufferedReader br;
					String riga="";
					int presenzeInRiga=0;
                	
                	for(int i=0;i<listaFile.length;i++) {
                		
                		try{if(listaFile[i].isFile()) {
                			if(listaFile[i].getName().endsWith(".txt")) {
                				System.out.println(listaFile[i].getPath());
                				br=new BufferedReader(new FileReader(listaFile[i]));
                				
                    			while((riga=br.readLine())!=null) {
                    				presenzeInRiga=0;
                    				for(int j=0;j<riga.length();j++) {
                        				if(riga.charAt(j)==carattere) {
                        					presenzeInRiga++;
                        				}
                        			}
                        			if(presenzeInRiga>=numOccorrenze) {
                        				result++;
                        			}	
                    			}
                    			
                			}
                			
                		}
                		}catch(Exception e) {
                			result=-1;
                		}
                		
                	}
                	outSock.writeInt(result);
                    
                //FINE
                } else if (selezione.equals("S")){//trasferimento tutti file binari di un direttorio
                //SERVIZIO
                	//T prezzo
                	String direttorio=inSock.readUTF();
                	DataInputStream is;
					int buffer;
					long fileSize=0;
					File directory=new File(direttorio);
					System.out.println(directory.getAbsolutePath());
					File[] listaFile=directory.listFiles();
						
							//outSock.writeInt(nFile);
							
					for(int j=0;j<listaFile.length;j++) {
						if(listaFile[j].isFile()) {	
                			if(!listaFile[j].getName().endsWith(".txt")) {
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
