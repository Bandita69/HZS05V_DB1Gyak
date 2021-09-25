/*
 ============================================================================
 Name        : binaris_lekerdezesek.c
 Author      : HZS05v
 Version     :
 Copyright   : Your copyright notice
 Description :
 Az autókat tároló adatfile-ban végezze el az alábbi lekérdezési mûveleteket:
 Számítsa ki a fájlban eltárolt autók átlagárát.
 Kérdezze le az eltárolt piros autók darabszámát.
 Keresse meg a legdrágább autót a fájlban.

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
// pl 1000000 ABC-123 Opel Kek

void teljes_record(FILE*, struct aut_record);

int atlag_ar(struct aut_record, FILE*);

int legdragabb_auto(struct aut_record, FILE*);

int piros_auto(struct aut_record, FILE*);

int main(int argc, char **argv) {
	FILE *f;

	char record_file_name[] = "records_auto";
	struct aut_record r;
	f = fopen(record_file_name, "rb");
	if (!f) {
		perror("Error opening input file: ");
		return 1;
	}
	teljes_record(f, r);
	//Számítsa ki a fájlban eltárolt autók átlagárát.
	int atlag = atlag_ar(r, f);

	printf("\nAz autok atlagara: %d", atlag);
	rewind(f);

	//Kérdezze le az eltárolt piros autók darabszámát. |piros auto|

	int piros = piros_auto(r, f);
	printf("\nPiros autok szama: %d", piros);
	rewind(f);

	//Keresse meg a legdrágább autót a fájlban.

	int legdragabb = legdragabb_auto(r, f);
	printf("\nA legdragabb auto %d-be kerul.", legdragabb);

	fclose(f);

}

int piros_auto(struct aut_record aut_struct, FILE *f) {

	int sum = 0;

	fseek(f, 0, SEEK_SET);
	while ((fread(&aut_struct, sizeof(struct aut_record), 1, f))) {

		if (strcmp(aut_struct.szin, "Piros") == 0) {
			sum++;
		}

	}

	return sum;

}

int legdragabb_auto(struct aut_record aut_struct, FILE *f) {

	int max = 0;
	//file elejere ugras. (lehet felesleges)
	fseek(f, 0, SEEK_SET);
	while ((fread(&aut_struct, sizeof(struct aut_record), 1, f))) {

		if (aut_struct.ar > max) {
			max = aut_struct.ar;

		}

	}

	return max;

}

int atlag_ar(struct aut_record aut_struct, FILE *f) {

	int atlag_Ar = 0;
	int sum = 0;
	fseek(f, 0, SEEK_SET);
	while ((fread(&aut_struct, sizeof(struct aut_record), 1, f))) {

		atlag_Ar += aut_struct.ar;
		sum++;

	}

	return atlag_Ar / sum;

}

void teljes_record(FILE *f, struct aut_record teszt) {

	while (fread(&teszt, sizeof(struct aut_record), 1, f)) {
		printf("%d %.3s-%s %s %s\n", teszt.ar, teszt.rendszam,
				teszt.rendszam + 3, teszt.tipus, teszt.szin);
	}

}
