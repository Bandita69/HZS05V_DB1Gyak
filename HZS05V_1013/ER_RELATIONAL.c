/*
 ============================================================================
 Name        : ER_RELATIONAL.c
 Author      : HZS05V
 Version     : 0.1
 Copyright   : Your copyright notice
 Description :
Egy egyed deklaracioja leirassal:

E(Student)
 egyed - neve
A(PKEY,Rollno)
 attributum - tipusa (PKEY- primary key, SIMA - default attributum, OSZP - osszetett attributum, OSZC - osszetett attributum tagjai/gyerekei, MULT - tobberteku attributum) , neve
A(SIMA,Class)
A(OSZP,Student Name)
A(OSSC,First Name)
A(OSSC,Last Name)
A(MULT,Contact Number)
R(1TOM,Rollno,Course Code)
 kapcsolat - tipusa (1TOM - 1 to many, NTOM - N to M, 1TO1, 1 TO 1) baloldalon allo egyed primary attributuma, jobboldalon allo egyed primary attributuma

Megjegyzes: egy adott kapcsolatot,egyefelekeppen ,mindket oldalon levo egyed deklaracioja utan jegyezni kell
 ============================================================================
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE_LENGTH 150

void valogatas(FILE*);

int main() {
	FILE *fp;

	char record_file_name[] = "ER.txt";
	fp = fopen(record_file_name, "r");
	if (!fp) {
		perror("Error opening input file: ");
		return 1;
	}
	valogatas(fp);

	fclose(fp);

	return 1;

}

void valogatas(FILE *fp) {

	char line[MAX_LINE_LENGTH];
	int i;
	char PKEY[15] = "";
	char *egyed_nev;
	char egyed_neve[15] = "";
	char *atribute_nev;
	char *r_ptr;
	char *r_tmp;
	char r_delim[] = ",";
	char tipus[5] = "";
	char atribute_nevek[100] = "";
	char multi_egyedek[100][100];
	char teljes[100] = "";

	//egyszeru file-ba iras
	freopen("output.txt", "w+", stdout);

	int j = 0;
	while ((i = fgetc(fp)) != EOF) {
		switch (i) {
		case 'E':

			fgets(line, MAX_LINE_LENGTH, fp);
			egyed_nev = strtok(line, "()");

			strcpy(egyed_neve, egyed_nev);

			break;

		case 'A':

			fgets(line, MAX_LINE_LENGTH, fp);

			atribute_nev = strtok(line, "()");
			strncpy(tipus, atribute_nev, 4);

			if (strcmp(tipus, "PKEY") == 0) {
				strcat(PKEY, atribute_nev + 5);
				strcat(atribute_nevek, atribute_nev + 5);
				strcat(atribute_nevek, " ,");
				break;

			}

			if (strcmp(tipus, "OSZC") == 0) {
				strcat(atribute_nevek, atribute_nev + 5);
				strcat(atribute_nevek, " ,");
				break;

			}

			if (strcmp(tipus, "SIMA") == 0) {
				strcat(atribute_nevek, atribute_nev + 5);
				strcat(atribute_nevek, " ,");
				break;

			}

			if (strcmp(tipus, "MULT") == 0) {

				snprintf(multi_egyedek[j], sizeof(multi_egyedek[j]),
						"%sTobbErteku%d(%s ,%s )", egyed_neve, j + 1, PKEY,
						atribute_nev + 5);
				j++;
				break;

			}

			break;

		case 'R':

			fgets(line, MAX_LINE_LENGTH, fp);

			atribute_nev = strtok(line, "()");

			strncpy(tipus, atribute_nev, 4);

			r_ptr = strtok(atribute_nev, r_delim);

			if (strcmp(tipus, "1TOM") == 0) {
				r_ptr = strtok(NULL, r_delim);
				r_ptr = strtok(NULL, r_delim);

				if (strcmp(PKEY, r_ptr) != 0) {

					strcat(atribute_nevek, r_ptr);
					strcat(atribute_nevek, " ,");

				}
			}

			if (strcmp(tipus, "1TO1") == 0) {

				r_ptr = strtok(NULL, r_delim);

				while (strcmp(PKEY, r_ptr) == 0) {
					r_ptr = strtok(NULL, r_delim);

				}
				strcat(atribute_nevek, r_ptr);
				strcat(atribute_nevek, " ,");

			}

			if (strcmp(tipus, "NTOM") == 0) {

				r_ptr = strtok(NULL, r_delim);
				r_tmp = r_ptr;
				r_ptr = strtok(NULL, r_delim);
				if (strcmp(PKEY, r_tmp) == 0) {

					snprintf(multi_egyedek[j], sizeof(multi_egyedek[j]),
							"NM_Kapcsolat(%s ,%s )", r_tmp, r_ptr);
					j++;

				}

			}

			break;

		case '\n':
			atribute_nevek[strlen(atribute_nevek) - 1] = '\0';
			strcat(atribute_nevek, ")");
			snprintf(teljes, sizeof(teljes), "%s(%s", egyed_neve,
					atribute_nevek);
			printf("\n%s", teljes);
			int k;
			for (k = 0; k < j; ++k) {
				printf("\n%s", multi_egyedek[k]);

			}

			j = 0;
			strcpy(PKEY, "");
			strcpy(atribute_nevek, "");
			printf("\n");

			break;
		default:

			break;

		}

	}

	atribute_nevek[strlen(atribute_nevek) - 1] = '\0';
	strcat(atribute_nevek, ")");
	snprintf(teljes, sizeof(teljes), "%s(%s", egyed_neve, atribute_nevek);
	printf("\n%s", teljes);
	int k;
	for (k = 0; k < j; ++k) {
		printf("\n%s", multi_egyedek[k]);

	}
	fprintf(stderr, "Done!");

}

