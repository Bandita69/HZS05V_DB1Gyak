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
	char *rendszam;
	char *tipus;

};
#define recordok_szama 10
#define leghosszabb_tipus 10
#define rendszam_hossz 7

void teljes_record(struct aut_record, FILE*);

void i_edik_record(struct aut_record, FILE*, int);

int main() {

	FILE *f;
	struct aut_record r;

	char record_file_name[] = "records_auto";

	int arak[] = { 1000000, 2000000, 3000000 };
	char tipusok[recordok_szama][leghosszabb_tipus] = { "Opel", "Peugeot",
			"Ferrari", "Lada", "Trabant"

	};

	char rendszamok[recordok_szama + 1][rendszam_hossz] = { "ABC123", "BCA321",
			"CCA123", "ASD321", "AAA111", "BBB222", "CCC765", "VVV432",
			"KKK333", "GGD333", "GGA333",

	};

	f = fopen(record_file_name, "w");
	if (!f) {
		perror("Error opening input file: ");
		return 1;
	}

	int j;
	// binarisfile 'veletlen' feltoltese recordokkal
	for (j = 1; j <= recordok_szama; j++) {
		r.ar = arak[j % 3];
		r.tipus = tipusok[j % 5];
		r.rendszam = rendszamok[j];

		fwrite(&r, sizeof(struct aut_record), 1, f);
	}
	fclose(f);

	f = fopen(record_file_name, "r");
	if (!f) {
		perror("Error opening input file: ");
		return 1;
	}

	teljes_record(r, f);

	printf("\n");

	int i;

	i = 3;
	//file elejere ugras
	rewind(f);

	i_edik_record(r, f, i);

	fclose(f);

	return 1;
}

void teljes_record(struct aut_record r, FILE *f) {
	int j;
	for (j = 1; j <= recordok_szama; j++) {
		fread(&r, sizeof(struct aut_record), 1, f);
		printf("%d:ft %s %.3s-%s \n", r.ar, r.tipus, r.rendszam,
				r.rendszam + 3);
	}
}

void i_edik_record(struct aut_record r, FILE *f, int i) {
	int j;
	for (j = 1; j <= i; j++) {
		fread(&r, sizeof(struct aut_record), 1, f);
	}
	printf("%d-edik record: %d:ft %s %.3s-%s \n", i, r.ar, r.tipus, r.rendszam,
			r.rendszam + 3);

}

