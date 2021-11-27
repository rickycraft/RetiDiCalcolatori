package server;

import java.rmi.Remote;
import java.rmi.RemoteException;

public class RegistryRemotoTagImpl implements RegistryRemotoTagServer {

	// Implementando RegistryRemotoTagServer implemento tutto perchè un'interfaccia
	// estende le altre che a sua volta estendono altre ancora

	private int grandezza = 100;
	private int ultimaEntry;

	private Object[][] map = new Object[grandezza][3];
	// 3 per nome_logico, rif_remoto e tag

	// Formato nel seguente modo:
	// NOME_LOGICO, REMOTE , TAG

	public RegistryRemotoTagImpl() {
		super();
		// un indice per indica l'ultima riga della tabella cosi da evitare di ciclare
		// ogni volta 100 volte
		ultimaEntry = -1;
		for (int i = 0; i < grandezza; i++) {
			map[i][0] = null;
			map[i][1] = null;
			map[i][2] = null;
		}
	}

	@Override
	public synchronized boolean aggiungiServer(String nome_logico, Remote rif_remoto) throws RemoteException {
		boolean result = false;
		if (nome_logico.isEmpty() || rif_remoto == null)
			throw new RemoteException("nome logico o riferimento non valido");
		else {
			/*
			 * for (int i = 0; i < ultimaEntry; i++) { if (map[i][0] == null) { map[i][0] =
			 * nome_logico; map[i][1] = rif_remoto; return true; } }
			 */
			ultimaEntry++;
			map[ultimaEntry][0] = nome_logico;
			map[ultimaEntry][1] = rif_remoto;

			result = true;
		}

		return result;
	}

	@Override
	public boolean eliminaPrimo(String nome_logico) throws RemoteException {
		boolean result = false;
		if (nome_logico.isEmpty())
			throw new RemoteException("nome non corretto");
		else {/*
				 * for (int i = 0; i < grandezza; i++) { if (map[i][0].equals(nome_logico)) {
				 * map[i][0] = null; map[i][1] = null; map[i][2] = null; return true; } }
				 */
			for (int i = 0; i < ultimaEntry + 1; i++) {
				if (map[i][0].equals(nome_logico)) {
					// invece di metterlo null, lo sostituisco con ultima entry
					map[i][0] = map[ultimaEntry][0];
					map[i][1] = map[ultimaEntry][1];
					map[i][2] = map[ultimaEntry][2];

					map[ultimaEntry][0] = null;
					map[ultimaEntry][1] = null;
					map[ultimaEntry][2] = null;
					ultimaEntry--;
					result = true;
				}
			}
		}
		return result;
	}

	@Override
	public Remote cerca(String nomeLogico) throws RemoteException {
		/*
		 * if (nomeLogico.isEmpty()) throw new RemoteException(); else { for (int i = 0;
		 * i < grandezza; i++) { if (map[i][0].equals(nomeLogico)) { return (Remote)
		 * map[i][1]; } }
		 * 
		 * }
		 */
		if (nomeLogico.isEmpty())
			throw new RemoteException();
		else {
			for (int i = 0; i < ultimaEntry + 1; i++) {
				if (map[i][0].equals(nomeLogico)) {
					return (Remote) map[i][1];
				}
			}

		}

		return null;
	}

	@Override
	public Remote[] cercaTutti(String nomeLogico) throws RemoteException {
		/*
		 * // Qua secondo me sarebbe meglio fare in modo che magari faccia un primo
		 * ciclo // per vedere quanti remote ci sono e poi fare un array // della
		 * grandezza giusta ma potrebbe cagare il cazzo perchè farebbe due cicli if
		 * (nomeLogico.isEmpty()) throw new RemoteException(); else {
		 * 
		 * int j = 0; for (int i = 0; i < grandezza; i++) { if
		 * (map[i][0].equals(nomeLogico)) { j++; } } if (j == 0) return null;
		 * 
		 * Remote[] result = new Remote[j]; j = 0; for (int i = 0; i < grandezza; i++) {
		 * if (map[i][0].equals(nomeLogico)) { result[j] = (Remote) map[i][1]; j++; } }
		 * return result; }
		 */
		if (nomeLogico.isEmpty())
			throw new RemoteException("nome non valido");
		else {

			int count = 0;
			for (int i = 0; i < ultimaEntry + 1; i++) {
				if (map[i][0].equals(nomeLogico)) {
					count++;
				}
			}
			if (count == 0)
				return null;

			Remote[] result = new Remote[count];
			count = 0;
			for (int i = 0; i < ultimaEntry + 1; i++) {
				if (map[i][0].equals(nomeLogico)) {
					result[count] = (Remote) map[i][1];
					count++;
				}
			}
			return result;
		}

	}

	@Override
	public String[] cercaTag(String tag) throws RemoteException {
		// Anche qui vale lo stesso commento di cercaTutti
		/*
		 * if (tag.isEmpty()) throw new RemoteException(); else { int j = 0; for (int i
		 * = 0; i < grandezza; i++) { if (map[i][2].equals(tag)) { j++; } } if (j == 0)
		 * return null; String[] result = new String[j]; j = 0; for (int i = 0; i <
		 * grandezza; i++) { if (map[i][2].equals(tag)) { result[j] = (String)
		 * map[i][2]; j++; } } return result; }
		 */
		if (tag.isEmpty())
			throw new RemoteException("tag non valido");
		else {
			int count = 0;
			for (int i = 0; i < ultimaEntry + 1; i++) {
				if (map[i][2].equals(tag)) {
					count++;
				}
			}
			if (count == 0)
				return null;
			String[] result = new String[count];
			count = 0;
			for (int i = 0; i < ultimaEntry + 1; i++) {
				if (map[i][2].equals(tag)) {
					result[count] = (String) map[i][2];
					count++;
				}
			}
			return result;
		}
	}

	@Override
	public boolean associaTag(String nome_logico_server, String tag) throws RemoteException {
		boolean result = false;
		if ((nome_logico_server.isEmpty()) || (tag.isEmpty())) {
			throw new RemoteException("nome logico o tag non valido");
		}
		for (int i = 0; i < ultimaEntry + 1; i++) {
			if (map[i][0].equals(nome_logico_server)) {
				map[i][2] = tag;
				result = true;
			}
		}
		return result;
	}

	@Override
	public Object[][] restituisciTutti() throws RemoteException {
		/*
		 * int count = 0; for (int i = 0; i < this.grandezza; i++) { if (map[i][0] !=
		 * null) { count++; } } if (count == 0) throw new RemoteException();
		 * 
		 * else { Object[][] result = new Object[count][3];
		 * 
		 * count = 0; for (int i = 0; i < this.grandezza; i++) { if (map[i][0] != null)
		 * {
		 * 
		 * result[count][0] = map[i][0]; result[count][1] = map[i][1]; result[count][2]
		 * = map[i][2]; count++; } }
		 * 
		 * return result;
		 * 
		 * }
		 */
		int count = 0;
		for (int i = 0; i < ultimaEntry + 1; i++) {
			if (map[i][0] != null) {
				count++;
			}
		}
		if (count == 0)
			throw new RemoteException("oggetto vuoto");

		else {
			Object[][] result = new Object[count][2];

			count = 0;
			for (int i = 0; i < ultimaEntry + 1; i++) {
				// non esistono righe vuote
				result[count][0] = map[i][0];
				result[count][1] = map[i][1];
				count++;
			}

			return result;

		}

	}

	@Override
	public boolean eliminaTutti(String nome_logico) throws RemoteException {
		/*
		 * int numEliminati = 0; if (nome_logico.isEmpty()) throw new RemoteException();
		 * else { for (int i = 0; i < ultimaEntry+1; i++) { if
		 * (map[i][0].equals(nome_logico)) { map[i][0] = null; map[i][1] = null;
		 * map[i][2] = null; numEliminati++; } }
		 * 
		 * }
		 * 
		 * if (numEliminati != 0) return true; return false;
		 */

		boolean result = false;
		if (nome_logico.isEmpty())
			throw new RemoteException("nome logico non valido");
		else {
			for (int i = 0; i < ultimaEntry + 1; i++) {
				if (map[i][0].equals(nome_logico)) {
					map[i][0] = map[ultimaEntry][0];
					map[i][1] = map[ultimaEntry][1];
					map[i][2] = map[ultimaEntry][2];

					map[ultimaEntry][0] = null;
					map[ultimaEntry][1] = null;
					map[ultimaEntry][2] = null;
					ultimaEntry--;
					result = true;
				}
			}
		}

		return result;
	}

}
