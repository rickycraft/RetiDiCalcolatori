/* FileUtility.java */
package utility;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.EOFException;
import java.io.IOException;

public class FileUtility {

	/**
	 * Nota: sorgente e destinazione devono essere correttamente aperti e chiusi da
	 * chi invoca questa funzione.
	 * 
	 */
	public static void trasferisci_a_byte_file_binario(DataInputStream src, DataOutputStream dest) throws IOException {

		// ciclo di lettura da sorgente e scrittura su destinazione
		int buffer;
		try {
			// esco dal ciclo all lettura di un valore negativo -> EOF
			// N.B.: la funzione consuma l'EOF
			while ((buffer = src.read()) >= 0) {
				dest.write(buffer);
			}
			dest.flush();
		} catch (EOFException e) {
			System.out.println("Problemi, i seguenti: ");
			e.printStackTrace();
		}
	}
}