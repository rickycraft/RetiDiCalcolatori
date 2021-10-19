package server;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;

public class LineUtility {

	static int swapLine(String filename, int nlinea1, int nlinea2) {

		if (nlinea1 == nlinea2)
			return 1; // Non occorre scambio

		try {
			String linea1 = null, linea2 = null, lineaCorrente;
			BufferedReader in = new BufferedReader(new FileReader(filename));
			int i = 1;

			while ((lineaCorrente = in.readLine()) != null) {
				if (i == nlinea1) {
					linea1 = lineaCorrente;
				} else if (i == nlinea2) {
					linea2 = lineaCorrente;
				}
				i++;
			}
			if (linea1 == null || linea2 == null) {
				in.close();
				throw new IOException("line not found");
			}
			in.close();

			File tmpFile = new File(filename + ".tmp");
			File inFile = new File(filename);
			in = new BufferedReader(new FileReader(inFile));
			PrintWriter printWriter = new PrintWriter(new FileWriter(tmpFile));

			i = 1;
			while ((lineaCorrente = in.readLine()) != null) {
				if (i == nlinea1) {
					printWriter.println(linea2);
				} else if (i == nlinea2) {
					printWriter.println(linea1);
				} else {
					printWriter.println(lineaCorrente);
				}
				i++;
			}

			printWriter.close();
			in.close();
			inFile.delete();
			tmpFile.renameTo(inFile);

			return 1;
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
