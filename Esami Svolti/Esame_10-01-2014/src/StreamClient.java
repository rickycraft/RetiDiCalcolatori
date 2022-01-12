
import java.io.*;
import java.net.*;
public class StreamClient {
    public static void main(String[] args) {
        InetAddress addr = null;
        int port = -1;

        Socket socket = null;
        DataInputStream inSock = null;
        DataOutputStream outSock = null;
        int inputCorretto=0;
        
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
            System.out.println("Servizi: L(eventi di un certo tipo in un determinato luogo)\t P(eventi disponibili a un prezzo <= a quello dato)\t ^D per terminare");
            
            while ((buffer = stdIn.readLine()) != null) {
                //operazione di Elimina
                if (buffer.equals("L")) {
                    System.out.println("Inserisci il nome del luogo e il tipo\n");
                    while(inputCorretto==0) {
                    	buffer=stdIn.readLine();
                        if(buffer.split(" ")[1].equals("Concerto") || buffer.split(" ")[1].equals("Formula1")  || buffer.split(" ")[1].equals("Calcio") ) {
                        	inputCorretto=1;
                        }	
                    }
                    outSock.writeUTF("L "+buffer);

                    //risposta dal server
                    System.out.println(inSock.readUTF());

                } else if (buffer.equals("P")) {
                	System.out.println("Inserisci il prezzo massimo\n");
                	inputCorretto=1;
                	do {
                    	buffer=stdIn.readLine();
                        try {
                        	Integer.parseInt(buffer);
                        }catch(NumberFormatException e) {System.out.println("Devi inserire correttamente il numero\n");} 
                        finally {inputCorretto=1;}
                	}while(inputCorretto==0);
	
                    outSock.writeUTF("P "+buffer);
                    

                    //risposta dal server
                    System.out.println(inSock.readUTF());

                } else
                    System.out.println("Nessun metodo corrispondente");

                System.out.println("Servizi: L(eventi di un certo tipo in un determinato luogo)\t P(eventi disponibili a un prezzo <= a quello dato)\t ^D per terminare");

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