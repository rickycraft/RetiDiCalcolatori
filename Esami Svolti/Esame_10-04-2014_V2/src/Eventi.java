
public class Eventi {

	private String tipo;
	private String id;
	private String luogo;
	private String data;
	private int numBiglietti;
	private int prezzo;
	
	public Eventi(String id,String tipo, String luogo, String data, int numBiglietti, int prezzo) {
		super();
		this.tipo = tipo;
		this.id = id;
		this.luogo = luogo;
		this.data = data;
		this.numBiglietti = numBiglietti;
		this.prezzo = prezzo;
	}
	public String getTipo() {
		return tipo;
	}
	public void setTipo(String tipo) {
		this.tipo = tipo;
	}
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}
	public String getLuogo() {
		return luogo;
	}
	public void setLuogo(String luogo) {
		this.luogo = luogo;
	}
	public String getData() {
		return data;
	}
	public void setData(String data) {
		this.data = data;
	}
	public int getNumBiglietti() {
		return numBiglietti;
	}
	public void setNumBiglietti(int numBiglietti) {
		this.numBiglietti = numBiglietti;
	}
	public int getPrezzo() {
		return prezzo;
	}
	public void setPrezzo(int prezzo) {
		this.prezzo = prezzo;
	}
	
	
}
