import java.io.Serializable;

public class Oggetto implements Serializable {

  public String proprieta;

  public Oggetto() {
    proprieta = "vuota";
  }

  public void editProp(String newProp) {
    this.proprieta = newProp;
  }

  public void stampa() {
    System.out.println("Oggetto [ proprieta: " + proprieta + " ]");
  }

}
