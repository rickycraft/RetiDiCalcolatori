package server;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

public class LineUtility {

	public static void main(String[] args) {

		// nomefile line1 line2
		System.out.println(LineUtility.swapLine(args[0], Integer.parseInt(args[1]), Integer.parseInt(args[2])));
	}

	static int swapLine(String filename, int line1, int line2) {

		if (line1 == line2)
			return 1; // Non occorre scambi

		String linea1 = LineUtility.getLine(filename, line1);
		String linea2 = LineUtility.getLine(filename, line2);

		if (linea1.equalsIgnoreCase("Linea non trovata"))
			return -1; // line1 non è presente nel file
		if (linea2.equalsIgnoreCase("linea non trovata"))
			return -1; // line2 non è presente nel file
		try {
			BufferedReader in = new BufferedReader(new FileReader(filename));

			BufferedWriter out = new BufferedWriter(new FileWriter(filename));
			int i = 1;
			boolean scambio1, scambio2;
			scambio1 = false;
			scambio2 = false;
			String lineaCorrente;
			while ((lineaCorrente = in.readLine()) != null) {
				if (i == line1) {
					out.write(linea2);
					scambio1 = true;
				} else {
					if (i == line2) {
						out.write(linea1);
						scambio2 = true;
					} else {
						out.write(lineaCorrente);
					}
				}

				i++;
			}
			out.close();
			in.close();

			if (scambio1 == true && scambio2 == true)
				return 1;
			else
				return -1;

		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return -1;
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
