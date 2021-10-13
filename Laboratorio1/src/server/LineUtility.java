package server;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

public class LineUtility {

	static int swapLine(String filename, int line1, int line2) {

		int primaLinea;
		if (line1 == line2)
			return -1;

		String linea1 = LineUtility.getLine(filename, line1);
		String linea2 = LineUtility.getLine(filename, line2);

		return 0;
	}

	static String getLine(String nomeFile, int numLinea) {
		String linea = null;
		BufferedReader in = null;
		try {
			in = new BufferedReader(new FileReader(nomeFile));
			for (int i = 1; i <= numLinea; i++) {
				linea = in.readLine();
				if (linea == null) {
					linea = "Linea non trovata";
					in.close();
					return linea;
				}
			}
			in.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
			return linea = "File non trovato";
		} catch (IOException e) {
			e.printStackTrace();
			return linea = "Linea non trovata";
		}
		return linea;
	}

	static String getNextLine(BufferedReader in) {
		String linea = null;
		try {
			if ((linea = in.readLine()) == null) {
				in.close();
				linea = "Nessuna linea disponibile";
			}
		} catch (FileNotFoundException e) {
			e.printStackTrace();
			return linea = "File non trovato";
		} catch (IOException e) {
			e.printStackTrace();
			linea = "Nessuna linea disponibile";
		}
		return linea;
	}

}
