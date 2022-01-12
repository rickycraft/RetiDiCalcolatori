
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
                
                if (input[0].equals("L")) {
                //SERVIZIO
                	//P luogo tipo
                	String result="";
                	for(int i=0;i<dati.length;i++) {
                		if(dati[i][2].equals(input[1])) {
                		
                			if(dati[i][1].equals(input[2])) {
                        		result+=dati[i][0]+" ";
                				result+=dati[i][1]+" ";
                				result+=dati[i][2]+" ";
                				result+=dati[i][3]+" ";
                				result+=dati[i][4]+" ";
                				result+=dati[i][5]+"\n";
                    		}
                		}
                		
                	}
                	outSock.writeUTF(result);
                    
                //FINE
                } else if (input[0].equals("P")){
                //SERVIZIO
                	//T prezzo
                	String result="";
                	for(int i=0;i<dati.length;i++) {
                		if(!dati[i][0].equals("L")) {
                		
                			if(Integer.parseInt(dati[i][5])<=Integer.parseInt(input[1])) {
                				result+=dati[i][0]+" ";
                				result+=dati[i][1]+" ";
                				result+=dati[i][2]+" ";
                				result+=dati[i][3]+" ";
                				result+=dati[i][4]+" ";
                				result+=dati[i][5]+"\n";
                    		}
                		}
                		
                	}
                	outSock.writeUTF(result);
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
