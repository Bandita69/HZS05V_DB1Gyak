/*
 ============================================================================
 Name        : TAFileOlvas.java   (nemtudom atnevezni az osztalyt 3.1.java-ra)
 Author      : HZS05v
 Version     : 0.2
 Copyright   : Your copyright notice
 Description : Írjon programot, amely egész típusú adatokat beolvassa a ,
kiszámítja az adatok összegét és kiírja a konzolra! Osztály neve: XYFileOlvas
A masodik feladat is benne van. txtbe ir ki es onnan olvas vissza.
 ============================================================================
 */

package minden;

import java.io.FileWriter;
import java.io.IOException;
import java.io.File; 
import java.io.FileNotFoundException; 

import java.util.Scanner;

public class HZS05V_3_1 {

	public final static int MAX_SZAMOK = 3;

	public static void main(String[] args) {

		// adatbazis feltoltese 3 adattal
		int numbers[] = { 10, 20, 30 };

		try {
			FileWriter myWriter = new FileWriter("vezeteknev.txt");

			for (int i = 0; i < 3; i++) {
				myWriter.write(numbers[i] + "\n" + "");
			}

			myWriter.close();

		} catch (IOException e) {
			System.out.println("Error");
			e.printStackTrace();
		}

		int Adat_osszeg = 0;
		int adat_szam = 0;
		int i = 0;
		int datas[] = new int[MAX_SZAMOK];
		try {
			File myObj = new File("vezeteknev.txt");
			Scanner myReader = new Scanner(myObj);

			while (myReader.hasNextLine()) {
				String data = myReader.nextLine();
				Adat_osszeg = Adat_osszeg + Integer.parseInt(data);
				adat_szam++;
				datas[i] = Integer.parseInt(data);
				i++;

			}
			myReader.close();
		} catch (FileNotFoundException e) {
			System.out.println("An error occurred.");
			e.printStackTrace();
		}
		System.out.println("Adatok szama: " + adat_szam);

		for (int j = 0; j < datas.length; j++) {

			System.out.println("A " + j + ". adat: " + datas[j]);

		}

		System.out.println("Osszeg: " + Adat_osszeg);
	}

}
