package server;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.net.InetAddress;

public class LineUtility {

	static int swapLine(String filename, int line1, int line2) {

		if (line1 == line2)
			return 1; // Non occorre scambio

		try {
			String linea1 = LineUtility.getLine(filename, line1);
			String linea2 = LineUtility.getLine(filename, line2);

			BufferedReader in = new BufferedReader(new FileReader(filename));
			StringBuilder out = new StringBuilder();
			int i = 1;
			boolean scambio1 = false;
			boolean scambio2 = false;
			String lineaCorrente;
			while ((lineaCorrente = in.readLine()) != null) {
				if (i == line1) {
					out.append(linea2 + "\n");
					scambio1 = true;
				} else {
					if (i == line2) {
						out.append(linea1 + "\n");
						scambio2 = true;
					} else {
						out.append(lineaCorrente + "\n");
					}
				}
				i++;
			}

			PrintWriter fout = new PrintWriter(new BufferedWriter(new FileWriter(filename)));
			fout.write(out.toString());
			fout.close();
			in.close();

			return (scambio1 == true && scambio2 == true) ? 1 : -1;
		} catch (FileNotFoundException e) {
			System.err.println("Non ho trovato il file");
		} catch (IOException e) {
			System.err.println("Errore di IO");
			e.printStackTrace();
		}
		return -1;
	}

	static String getLine(String nomeFile, int numLinea) throws FileNotFoundException, IOException {
		String linea = null;
		BufferedReader in = new BufferedReader(new FileReader(nomeFile));
		for (int i = 1; i <= numLinea; i++) {
			linea = in.readLine();
			if (linea == null) {
				in.close();
				throw new IOException("line not found");
			}
		}
	
		in.close();
		return linea;
	}

}
