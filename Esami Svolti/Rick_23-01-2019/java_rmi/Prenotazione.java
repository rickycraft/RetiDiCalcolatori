import java.io.Serializable;

public class Prenotazione implements Serializable {
  public String targa;
  public String patente;
  public String tipoVeicolo;
  public String cartellaImmagini;

  public Prenotazione() {
    targa = "L";
    patente = "0";
    tipoVeicolo = "L";
    cartellaImmagini = targa + "_img/";
  }

  public Prenotazione(String targa, String patente, String tipoVeicolo) {
    this.targa = targa;
    this.patente = patente;
    this.tipoVeicolo = tipoVeicolo;
    cartellaImmagini = targa + "_img/";
  }

  @Override
  public String toString() {
    return String.format("targa: %s tipoVeicolo: %s patente: %s", targa, tipoVeicolo, patente);
  }

  public boolean isLibera() {
    return (targa == "L" || patente == "0" || tipoVeicolo == "L");
  }
}
