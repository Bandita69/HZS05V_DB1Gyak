/* FELADAT:
 *
 * A szabvány billentyûzetrõl olvasson be sorokat, egészen a végjelig (#).A szövegfile nevét a bevitel elsõ sorában adja meg.
 *  A beolvasott sorokat írja ki egy szövegfile-ba .Az így létrehozott,
 *  lezárt állományt utána nyissa meg és írja vissza a lementett szöveget nagybetûs
 *  formában.
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
