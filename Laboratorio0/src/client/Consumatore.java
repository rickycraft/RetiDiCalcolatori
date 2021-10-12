package client;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.Reader;

// Consumatore e' un filtro
public class Consumatore {
	public static void main(String[] args) {
		FileReader r = null;

		if (args.length < 1 || args.length > 2) {
			System.out.println("Utilizzo: consumatore <inputFilename>");
			System.exit(0);
		}
		String filtro = args[0];

		if (args.length == 1) {
			BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
			filtra(filtro, in);

		}

		if (args.length == 2) {

			try {

				r = new FileReader(args[1]);
				filtra(filtro, r);
			} catch (FileNotFoundException e) {
				System.out.println("File non trovato");
				System.exit(1);
			}

		}

	}

	private static void filtra(String filtro, Reader r) {
		char ch;
		int x;

		try {
			while ((x = r.read()) >= 0) {
				ch = (char) x;
				if (filtro.indexOf(ch) < 0) {
					System.out.print(ch);
				}
			}
			r.close();
		} catch (IOException e) {
			System.out.println("File non trovato");
			System.exit(1);
		}
	}
}
