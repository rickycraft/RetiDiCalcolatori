// PutFileClient.java
package client;
import java.net.*;

import java.io.*;

public class PutFileClient {

	//voglio in ingresso ip, porta , limiteMinFile e directory 
	public static void main(String[] args) throws IOException {

		InetAddress addr = null;
		int port = -1;
		String directory=null;
		long sizeFile=-1;

		try{ //check args
			if(args.length != 4){
				System.out.println("Usage: java PutFileClient serverAddr serverPort directory grandezzaMinFile");
				System.exit(1);
			}
			else
			{
				addr = InetAddress.getByName(args[0]);	
				port = Integer.parseInt(args[1]);
				directory=args[2];
				sizeFile=Integer.parseInt(args[3]);
			}


		} //try
		// Per esercizio si possono dividere le diverse eccezioni
		catch(Exception e){
			System.out.println("Problemi, i seguenti: ");
			e.printStackTrace();
			System.out.println("Usage: java PutFileClient serverAddr serverPort");
			System.exit(2);
		}

		// oggetti utilizzati dal client per la comunicazione e la lettura del file
		// locale
		Socket socket = new Socket(addr,port);
		socket.setSoTimeout(300000);
		if(!socket.isConnected())
		{
			System.out.println("Socket non connesso al server");
			System.exit(-1);
		}
		File direct=new File(directory);
		String nomeFile;
		System.out.printf("IP e porta messi nella socket: %s %d",addr.getAddress().toString() , port);
		DataInputStream inSock=new DataInputStream(socket.getInputStream());
		DataOutputStream outSock=new DataOutputStream(socket.getOutputStream());
		String reply;

		for(File dir : direct.listFiles())
		{

			if(dir.isFile())
			{
				if(dir.length()>sizeFile)
				{
					
					FileInputStream fis=new FileInputStream(dir);
					long lunghezzaFile=dir.length();
					nomeFile=dir.getName();
					//Mando il nome del file
					outSock.writeUTF(nomeFile);
					//Ricevo risposta
					reply=inSock.readUTF();
					if(reply.equalsIgnoreCase("attiva"))
					{
						outSock.writeLong(lunghezzaFile);
						int buffer;
						while ((buffer=fis.read()) >= 0) {
							outSock.write(buffer);
							fis.close();	
						}
						outSock.flush();
					}		
				}
			}
		}
		socket.close();


	} // main
} // PutFileClient
