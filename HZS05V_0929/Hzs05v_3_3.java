/*
 ============================================================================
 Name        : filebaIroEndig.java
 Author      : HZS05v
 Version     :
 Copyright   : Your copyright notice
 Description : A szabvány billentyûzetrõl olvasson be sorokat, egészen a “end” szóig. A beolvasott sorokat
írja ki egy szövegfile-ba. A szövegfile nevét a bevitel elsõ sorában adja meg. 
 ============================================================================
 */


package teszt3dikfeladat;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.InputStreamReader;

public class Hzs05v_3_3 {


	public static void main(String[] args) {
		Hzs05v_3_3 teszt = new Hzs05v_3_3();
		teszt.Read_end();

	}
	
	
	public void Read_end() {
		
		String sor;
		String[] szavak;
		int sorId=0;
		
		try {
			BufferedReader bfr = new BufferedReader(new InputStreamReader(System.in));		
			BufferedWriter bfw = null;
			System.out.println("Name of the file: ");
			while (0 <= sorId) {
				
				sor = bfr.readLine();
				if (sorId == 0) {
					
					bfw = new BufferedWriter(new FileWriter(sor));
					System.out.println("Text of the file: ");
				
					
					
				}else {
					bfw.write(sor);
					bfw.newLine();
				}
				sorId ++;
				
				szavak = sor.split(" ");
				for (String string : szavak) {
					
					
					if (string.compareTo("end")==0) {
						
						bfr.close();
						sorId = -1;
						
					}
					
				}
				
				
			}
			bfw.close();
			System.out.println("\nEnded!");
			
		} catch (Exception e) {
			e.printStackTrace();
		}
		
	}

}
