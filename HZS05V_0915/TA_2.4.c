/*
 ============================================================================
 Name        : binaris_file_rekord_kesztito_olvaso.c
 Author      : HZS05v
 Version     :
 Copyright   : Your copyright notice
 Description : Tároljon le auto (rendszam, tipus, ar) rekordokat egymás után egy bináris állományban, majd
 készítsen függvényt az i. rekord visszaolvasására
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct aut_record {
	int ar;
	char rendszam[10];
	char tipus[10];
	char szin[10];

};

struct aut_record Data[10];

#define recordok_szama 10
#define leghosszabb_tipus 15
#define rendszam_hossz 7

void teljes_record(FILE*, struct aut_record);

void i_edik_record(struct aut_record, FILE*, int);
#define hanyadik 3


int main() {

	FILE *f;

	char record_file_name[] = "records_auto";

	int arak[] = { 1000000, 2000000, 3000000, 4000000 };

	char tipusok[recordok_szama][leghosszabb_tipus] = { "Opel", "Peugeot",
			"Ferrari", "Lada", "Trabant",

	};

	char rendszamok[recordok_szama + 1][rendszam_hossz] = { "ABC123", "BCA321",
			"CCA123", "ASD321", "AAA111", "BBB222", "CCC765", "VVV432",
			"KKK333", "GGD333", "GGA333",

	};

	char szinek[recordok_szama][6] = { "Kek", "Piros", "Sarga", "Zold",

	};

	f = fopen(record_file_name, "wb+");
	if (!f) {
		perror("Error opening input file: ");
		return 1;
	}

	int j;
	// binarisfile 'veletlen' feltoltese recordokkal

	for (j = 0; j < recordok_szama; ++j) {

		Data[j].ar = arak[j % 4];
		strcpy(Data[j].rendszam, rendszamok[j]);
		strcpy(Data[j].tipus, tipusok[j % 5]);
		strcpy(Data[j].szin, szinek[j % 4]);
		fwrite(&Data[j], sizeof(struct aut_record), 1, f);

	}

	fclose(f);

	f = fopen(record_file_name, "rb");
	if (!f) {
		perror("Error opening input file: ");
		return 1;
	}



	struct aut_record r;
	teljes_record(f, r);

	printf("\n");


	rewind(f);




	//file elejere ugras

	i_edik_record(r, f, hanyadik);

	fclose(f);

	return 1;
}

void teljes_record(FILE *f, struct aut_record teszt) {

	while (fread(&teszt, sizeof(struct aut_record), 1, f)) {
		printf("%d %.3s-%s %s %s\n", teszt.ar, teszt.rendszam,
				teszt.rendszam + 3, teszt.tipus, teszt.szin);
	}

}

void i_edik_record(struct aut_record r, FILE*f, int i) {
	int j;
	for (j = 1; j <= i; j++) {
		fread(&r, sizeof(struct aut_record), 1, f);
	}
	printf("Az %d -dik record : %d %.3s-%s %s %s\n",i, r.ar, r.rendszam,
					r.rendszam + 3, r.tipus, r.szin);

}

