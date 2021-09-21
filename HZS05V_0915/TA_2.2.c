/* FELADAT:
 *
 * A szabv�ny billenty�zetr�l olvasson be sorokat, eg�szen a v�gjelig (#).A sz�vegfile nev�t a bevitel els� sor�ban adja meg.
 *  A beolvasott sorokat �rja ki egy sz�vegfile-ba .Az �gy l�trehozott,
 *  lez�rt �llom�nyt ut�na nyissa meg �s �rja vissza a lementett sz�veget nagybet�s
 *  form�ban.
 */

#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h>

int main() {

	char fileName[20];
	FILE *pFileOut;
	printf("Adja meg a file nevet: ");

	//eclipse console bug miatt
	fflush( stdout);

	scanf("%s", fileName);
	strcat(fileName, ".txt");

	pFileOut = fopen(fileName, "w+");

	if (pFileOut == NULL)
		perror("Error opening output file: ");

	int ch;
	printf("Adja a szoveget. #-al kilep: \n");
	fflush( stdout);

	do {
		ch = getc(stdin);
		putc(ch, pFileOut);

	} while (ch != '#');

	fclose(pFileOut);

	printf("\nSzoveg ujra:");
	fflush( stdout);

	pFileOut = fopen(fileName, "r");
	while (1) {
		ch = fgetc(pFileOut);
		if (feof(pFileOut)) {
			break;
		}

		printf("%c", toupper(ch));
	}
	fclose(pFileOut);
	return (0);

}
