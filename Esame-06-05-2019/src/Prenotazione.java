import java.io.Serializable;

public class Prenotazione implements Serializable{

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private String targa;
	private String patente;
	private String tipoVeicolo;
	private String dir_immagini;
	
	public Prenotazione() {
		super();
		this.targa= "L";
		this.patente = "0";
		this.tipoVeicolo= "L";
		this.dir_immagini="L";
	}

	public void setPrenotazione(String targa,String patente,String tipoVeicolo,String di_Immagini) {

		this.targa= targa;
		this.patente = patente;
		this.tipoVeicolo= tipoVeicolo;
		this.dir_immagini=di_Immagini;

	}

	public String getTarga() {
		return targa;
	}

	public void setTarga(String targa) {
		this.targa = targa;
	}

	public String getPatente() {
		return patente;
	}

	public void setPatente(String patente) {
		this.patente = patente;
	}

	public String getTipoVeicolo() {
		return tipoVeicolo;
	}

	public void setTipoVeicolo(String tipoVeicolo) {
		this.tipoVeicolo = tipoVeicolo;
	}

	public String getDir_immagini() {
		return dir_immagini;
	}

	public void setDir_immagini(String dir_immagini) {
		this.dir_immagini = dir_immagini;
	}
	
	public void svuota() {
		this.targa= "L";
		this.patente = "0";
		this.tipoVeicolo= "L";
		this.dir_immagini="L";
	}
}
