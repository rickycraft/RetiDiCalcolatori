package client;

import server.DiscoveryServer;

public class Runner extends Thread {

	public static void main(String args[]) {
		// DS nomefile RS riga1 riga2

		String main[] = new String[3];
		main[0] = "127.0.0.1";
		main[1] = args[0];
		main[2] = args[1];
		String ds[] = new String[3];
		ds[0] = args[0];
		ds[1] = args[1];
		ds[2] = args[2];

		Client.main(main);
		DiscoveryServer.main(ds);
	}

}
