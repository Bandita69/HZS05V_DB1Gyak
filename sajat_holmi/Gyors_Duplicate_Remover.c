/*
 ============================================================================
 Name        : Gyors Dublicate Remover
 Author      : HZS05V
 Version     : 0.2
 Copyright   : -
 Description : Megnyit egy in.txt filet annak a sorait egyedi hash-al indexeli,
 majd �sszehasonl�tja a t�bbivel. Ha egyez�st tal�l azt nem m�solja az out.txt-be.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//linked list
struct hash_and_memo {
	struct hash_and_memo *next;
	unsigned stored_hash_index;
	char *hash_mem;
};

// legal�bb a k�l�nb�z� felhaszn�lt karakterek sz�ma + 2 (\n) pl. a b c karekterek eset�n = 5
// ha a console t�rdeli az eredm�nyt , vagy hib�san adja meg az out.txt-t akkor n�velni kell
#define magic_Karp_number 500

struct hash_and_memo *head[magic_Karp_number] = { NULL, };
struct hash_and_memo** tail(char *str, unsigned len);

static unsigned int rolling_hash_index(char *str, unsigned lenght);

int main(void) {
	char buffer[magic_Karp_number];
	struct hash_and_memo **prt;
	size_t len;
	FILE *pFileIn;
	FILE *pFileOut;

	//szukseg van egy in.txt nevu filera, az utols� sor v�g�n is legyen enter (\n)
	pFileIn = fopen("in.txt", "r");
	pFileOut = fopen("out.txt", "w+");

	//hibakezel�s
	if (pFileIn == NULL)
		perror("Error opening input file: ");
	if (pFileOut == NULL)
		perror("Error opening output file: ");

	while (fgets(buffer, magic_Karp_number, pFileIn)) {
		len = strlen(buffer);
		prt = tail(buffer, len);
		if (*prt) { //tal�lt
			printf("%s    ^ Duplik�l�st tal�ltam! \n", buffer);
		} else { /* nem tal�lt */
			printf("%s", buffer);
			fprintf(pFileOut, "%s", buffer);
			//dinamikus mem�ria allok�ci�
			*prt = malloc(sizeof **prt);
			(*prt)->next = NULL;
			(*prt)->stored_hash_index = rolling_hash_index(buffer, len);
			(*prt)->hash_mem = malloc(1 + len);
			memcpy((*prt)->hash_mem, buffer, 1 + len);
		}
	}

	return 0;
}

struct hash_and_memo** tail(char *str, unsigned int len) {
	unsigned int this_hash_index;
	unsigned short place_in_table;
	struct hash_and_memo **ham_ptr;

	this_hash_index = rolling_hash_index(str, len);
	place_in_table = this_hash_index % magic_Karp_number;
	for (ham_ptr = &head[place_in_table]; *ham_ptr; ham_ptr =
			&(*ham_ptr)->next) {
		// megn�zi hogy az adott helyen l�v� hash pointere l�tez� hash cimre mutat e, ha igen akkor break-eli a loopot
		if ((*ham_ptr)->stored_hash_index != this_hash_index)
			continue;



		break;
	}

	return ham_ptr;
}
//hashel�s, minden karakter v�ltoztatja a hash �rt�k�t, sorrendt�l f�gg�en, pl. ABA = 786418890 ; BAB = 2942192335 (min�l nagyobb ann�l kisebb az es�lye a hib�nak, �s a hashok �ssze�tk�z�s�nek, valamint jobban eloszt�dnak  a t�bl�ban)
static unsigned int rolling_hash_index(char *str, unsigned int lenght) {
	unsigned int value;
	unsigned int index;

	if (!lenght)
		lenght = strlen(str);

	value = 0;
	for (index = 0; index < lenght; index++) {
		value ^= (value >> 2) ^ (value << 5) ^ (value << 13) ^ str[index]
				^ 0x7FFFFFFF;
	}

	return value;
}

