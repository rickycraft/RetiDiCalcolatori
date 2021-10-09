package server;

import java.io.BufferedReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;

public class Produttore {

	public static void main(String[] args) {		
		
		if (args.length != 1){
			System.out.println("Utilizzo: produttore <inputFilename>");
			System.exit(0);
		}
		
		BufferedReader in = null;
		in = new BufferedReader(new InputStreamReader(System.in));
			
		FileWriter fout;
		String line = null;
		try {
			fout = new FileWriter(args[0]);
			line = in.readLine() + "\n";
			while (line != null) {
				fout.write(line, 0, line.length());
				line = in.readLine() + "\n";
			}
			fout.close();
		}
	    catch (IOException e) { 
			e.printStackTrace();
			System.exit(2); // uscita con errore, intero positivo a livello di sistema Unix
		}
	}

}
