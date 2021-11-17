package server;

public class FileRighe {

	private String nomeFile;
	private int nRighe;
	
	public FileRighe(String nomeFile,int nRighe) {
		this.nomeFile=nomeFile;
		this.nRighe=nRighe;
	}

	public String getNomeFile() {
		return nomeFile;
	}

	public void setNomeFile(String nomeFile) {
		this.nomeFile = nomeFile;
	}

	public int getnRighe() {
		return nRighe;
	}

	public void setnRighe(int nRighe) {
		this.nRighe = nRighe;
	}
	
	
}
