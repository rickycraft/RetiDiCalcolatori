
import java.io.*;
import java.net.*;
public class StreamClient {
    public static void main(String[] args) {
        InetAddress addr = null;
        int port = -1;

        Socket socket = null;
        DataInputStream inSock = null;
        DataOutputStream outSock = null;
        
        BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));

        String buffer;
     
        try {
            if (args.length == 2) {
                addr = InetAddress.getByName(args[0]);
                port = Integer.parseInt(args[1]);
                if (port < 1024 || port > 65535) {
                    System.out.println("[ERROR] la porta deve essere compresa nel range valido [1024-65535 ");
                    System.exit(1);
                }
            } else {
                System.out.println("[ERROR] java StreamClient serverAddr serverPort ");
                System.exit(1);
            }
        }
        catch (Exception e) {
            System.out.println("[ERROR] Problemi, i seguenti: ");
            e.printStackTrace();
            System.out.println("java StreamClient serverAddr serverPort ");
            System.exit(2);
        }

        // CREAZIONE SOCKET
        try {
            socket = new Socket(addr, port);
            socket.setSoTimeout(300000);
            System.out.println("[DEBUG] Creata la socket: " + socket + " sulla porta locale " + socket.getLocalPort());
        } catch (Exception e) {
            System.out.println("[ERROR] Problemi nella creazione della socket: ");
            e.printStackTrace();
            System.exit(1);

        }
        //CREAZIONE STREAM
        try {
            inSock = new DataInputStream(socket.getInputStream());
            outSock = new DataOutputStream(socket.getOutputStream());
        } catch (IOException e) {
            System.out.println("[ERROR] Problemi nella creazione degli stream su socket: ");
            e.printStackTrace();
            System.exit(1);
        }

        //CICLO INTERAZIONE CLIENT

        try {
            System.out.println("Servizi: E(elimina occorrenze )\t S(scarica file )\t ^D per terminare");
            
            while ((buffer = stdIn.readLine()) != null) {
                //operazione di Elimina
                if (buffer.equals("E")) {
                	outSock.writeUTF("E");
                	System.out.println("Inserisci il nome de file");
                    buffer=stdIn.readLine();
                    
                    outSock.writeUTF(buffer);
                    System.out.println("Inserisci la parola da eliminare\n");
                    buffer=stdIn.readLine();
                    
                    
                    outSock.writeUTF(buffer);
                    
                    //risposta dal server
                    int result=inSock.readInt();
                    if(result>=0) {
                    	System.out.println("Operazione eseguita con successo!\nHa trovato "+result+" occorrenze\n");
                    }else {
                    	System.out.println("Errore nella riuscita.\n");
	
                    }

                } else if (buffer.equals("S")) {
                	outSock.writeUTF("S");
                	boolean corretto=false;
                	System.out.println("Inserisci il direttorio\n");
                	buffer=stdIn.readLine();
                	while(!corretto) {
                		if(!(new File(buffer).isDirectory())) {
                    		System.out.println("Devi inserire un direttorio corretto.\n Riprova:");
                    		buffer=stdIn.readLine();
                    	}else {
                    		corretto=true;
                    	}	
                	}
                	outSock.writeUTF(buffer);
                	
                	System.out.println("Inserisci una soglia minima\n");
                	buffer=stdIn.readLine();
                	corretto=false;
                	int soglia=0;
                	
                	while(!corretto)
                	{
                		try {
                			soglia=Integer.parseInt(buffer);
                			corretto=true;
                		}catch(NumberFormatException e) {
                			System.out.println("DEVI INSERIRE UN NUMERO. Riprova\n");
                			buffer=stdIn.readLine();
                        			
                		}
                	}
                	outSock.writeInt(soglia);
                	
                	//non faccio buffer+clientFile perchè teoricamente la directory scritta nel buffer è del server.
                    File dir=new File(new File("").getAbsolutePath()+"/clientFile");
                    dir.mkdir();
                    System.out.println("dir:"+dir.getAbsolutePath());
                    String nomeFile;
                    File f;
                    DataOutputStream os = null;
                    boolean fileRimanenti=true;
                    //int nFile=inSock.readInt();
                    //System.out.println("NFILE:"+nFile);
                    while(fileRimanenti) {
                    	//System.out.println("I:"+i+"\n");
                        
                    	nomeFile=inSock.readUTF();
                    	if(!nomeFile.equals("FINE FILE")) {
                    		long lunghezza=inSock.readLong();
                        	System.out.println(dir.getPath()+"/"+nomeFile);
                            f=new File(dir.getPath()+"/"+nomeFile);
                            System.out.println(lunghezza);
                            os=new DataOutputStream(new FileOutputStream(f));
                            int b=0;
                            for(int j=0;j<lunghezza;j++) {
                            	//System.out.println("j:"+j+"\n");
                            	b=inSock.read();
                            	os.write(b);
                            }
                            System.out.println("File terminato\n");        
                           
                    	}else {
                    		fileRimanenti=false;
                    	}
                                                //f.setWritable(true);  
                    }
                    os.close();
                    //risposta dal server
                    
                } else
                    System.out.println("Nessun metodo corrispondente");

                System.out.println("Servizi: E(elimina occorrenze )\t S(scarica file )\t ^D per terminare");

            }

        } catch (Exception e1) {
            System.out.println("Problemi nel ciclo di interazione");
            e1.printStackTrace();
        }

        // fine ciclo

        //CHIUSURA SOCKET
        try {
            socket.shutdownOutput();
            socket.shutdownInput();
            socket.close();
        } catch (IOException e) {
            System.out.println("Problemi nella chiusura delle socket");
            e.printStackTrace();
            System.exit(1);
        }

    }// main

}