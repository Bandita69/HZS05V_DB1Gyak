/*
 ============================================================================
 Name        : benchmark.c
 Author      : HZS05v
 Version     : 0.2
 Copyright   : Your copyright notice
 Description : A hash és a b-fa keresési gyorsaságának mérése, a fa/tábla felépítését nem beleszámolva.
 -ciklus által való feltöltés
 -egy szám sokszor történõ  megkeresése
 -kapott sebességek birálása (clock() / CLOCKS_PER_SEC), nagyban függ a processzor terheltéségtõl.
 -keresések idejének összehasonlitása. A keresések átlaga nagyon kicsi szám ( < 0.000000 )
 -Tapasztalatok alapján: Legrosszabb eset esetén ( ha a szám nem található vagy a legnagyobb ) , kevesebb szám között való kereséskor (pl 1000000) a hash gyorsabb, de nagyobbaknál a b-tree.
 -Legjobb esetben ( ha az 1- számot keressük akkor minden esetben a hash gyorsabb )
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define legnagyobbSzam 2072756 // nálam a maximum 2072756 lehet, ha nem fut le akkor kisebbre kell venni

#define keresettSzam 2072756
#define keresesekSzama 1000000

// Hash-es dolgok--------------------------------------------------------------
struct hash_and_memo {
	struct hash_and_memo *next;
	unsigned stored_hash_index;
	char *hash_mem;
};

struct hash_and_memo *head[legnagyobbSzam] = { NULL, };
struct hash_and_memo** tail(char *str, unsigned len);

static unsigned int rolling_hash_index(char *str, unsigned lenght);

//BTREE-s dolgok----------------------------------------------------------------
#define LIMIT 3 // vagy "fok"
#define MIN_MEDIAN 2

struct BTreeNode {
	int val[LIMIT + 1], count;
	struct BTreeNode *link[LIMIT + 1];
};

struct BTreeNode *root;

struct BTreeNode* createNode(int, struct BTreeNode*);
void insertNode(int, int, struct BTreeNode*, struct BTreeNode*);
void splitNode(int, int*, int, struct BTreeNode*, struct BTreeNode*,
		struct BTreeNode**);
int setValue(int, int*, struct BTreeNode*, struct BTreeNode**);
void search(int, int*, struct BTreeNode*);
void insert(int);

int main() {

	double startTime;
	double endTime;
	int feltoltCiklus;
	int keresCiklus;

	printf("Planting trees... it may take a while \n");
	fflush(stdout);

	//------------------------------------------------------------
	//btree program:
	int ch;
	double timeElapsedBTree;

	for (feltoltCiklus = 0; feltoltCiklus <= legnagyobbSzam; ++feltoltCiklus) {

		insert(feltoltCiklus);

	}

	startTime = (double) clock() / CLOCKS_PER_SEC;

	for (keresCiklus = 0; keresCiklus <= keresesekSzama; keresCiklus++) {

		search(keresettSzam, &ch, root);
	}

	//btree program vege¡¡
	//-------------------------------------------------------------

	endTime = ((double) clock() / CLOCKS_PER_SEC);

	timeElapsedBTree = endTime - startTime;
	//-------------------------------------------------------------
	//Hash Program:
	double timeElapsedHash;

	char buffer[legnagyobbSzam];
	struct hash_and_memo **prt;
	size_t len;

	//ez feltölt
	for (feltoltCiklus = 0; feltoltCiklus <= legnagyobbSzam; feltoltCiklus++) {
		sprintf(buffer, "%d", feltoltCiklus);

		len = strlen(buffer);
		prt = tail(buffer, len);
		if (!*prt) {
			*prt = malloc(sizeof **prt);
			(*prt)->next = NULL;
			(*prt)->stored_hash_index = rolling_hash_index(buffer, len);
			(*prt)->hash_mem = malloc(1 + len);
			memcpy((*prt)->hash_mem, buffer, 1 + len);
		}
	}

	//ez keres
	sprintf(buffer, "%d", keresettSzam);

	startTime = (double) clock() / CLOCKS_PER_SEC;

	for (keresCiklus = 0; keresCiklus <= keresesekSzama; keresCiklus++) {
		len = strlen(buffer);
		prt = tail(buffer, len);

		if (!*prt) {
			printf("\n nem talalta meg a: %s -t", buffer);
			break;

		}
	}

	//------------------------------------------------------------------------
	// Hash program vege¡¡

	endTime = ((double) clock() / CLOCKS_PER_SEC);

	timeElapsedHash = endTime - startTime;



	printf("\nbtree ideje: %f és egy keresés átlaga: %f", timeElapsedBTree,
			timeElapsedBTree / keresesekSzama);
	printf("\nhash ideje:  %f és egy keresés átlaga: %f", timeElapsedHash,
			timeElapsedHash / keresesekSzama);

	return 0;
}
//Függvények:
//BTREE¡¡ (alatta van a hash)

struct BTreeNode* createNode(int val, struct BTreeNode *child) {
	struct BTreeNode *newNode;
	newNode = (struct BTreeNode*) malloc(sizeof(struct BTreeNode));
	newNode->val[1] = val;
	newNode->count = 1;
	newNode->link[0] = root;
	newNode->link[1] = child;
	return newNode;
}

void insertNode(int val, int pos, struct BTreeNode *node,
		struct BTreeNode *child) {
	int j = node->count;
	while (j > pos) {
		node->val[j + 1] = node->val[j];
		node->link[j + 1] = node->link[j];
		j--;
	}
	node->val[j + 1] = val;
	node->link[j + 1] = child;
	node->count++;
}

void splitNode(int val, int *pval, int pos, struct BTreeNode *node,
		struct BTreeNode *child, struct BTreeNode **newNode) {
	int median, j;

	if (pos > MIN_MEDIAN)
		median = MIN_MEDIAN + 1;
	else
		median = MIN_MEDIAN;

	*newNode = (struct BTreeNode*) malloc(sizeof(struct BTreeNode));
	j = median + 1;
	while (j <= LIMIT) {
		(*newNode)->val[j - median] = node->val[j];
		(*newNode)->link[j - median] = node->link[j];
		j++;
	}
	node->count = median;
	(*newNode)->count = LIMIT - median;

	if (pos <= MIN_MEDIAN) {
		insertNode(val, pos, node, child);
	} else {
		insertNode(val, pos - median, *newNode, child);
	}
	*pval = node->val[node->count];
	(*newNode)->link[0] = node->link[node->count];
	node->count--;
}

int setValue(int val, int *pval, struct BTreeNode *node,
		struct BTreeNode **child) {
	int pos;
	if (!node) {
		*pval = val;
		*child = NULL;
		return 1;
	}

	if (val < node->val[1]) {
		pos = 0;
	} else {
		for (pos = node->count; (val < node->val[pos] && pos > 1); pos--)
			;
		if (val == node->val[pos]) {
			//printf("Duplicates are not allowed\n");
			return 0;
		}
	}
	if (setValue(val, pval, node->link[pos], child)) {
		//ha kisebb mint a limit akkor beilleszti, ha nagyobb akkor kettéválasztja
		if (node->count < LIMIT) {
			insertNode(*pval, pos, node, *child);
		} else {
			splitNode(*pval, pval, pos, node, *child, child);
			return 1;
		}
	}
	return 0;
}

void insert(int val) {
	int flag, i;
	struct BTreeNode *child;

	flag = setValue(val, &i, root, &child);
	if (flag)
		root = createNode(i, child);
}

// levelérték kereses
void search(int val, int *pos, struct BTreeNode *myNode) {
	if (!myNode) {
		return;
	}

	if (val < myNode->val[1]) {
		*pos = 0;
	} else {
		for (*pos = myNode->count; (val < myNode->val[*pos] && *pos > 1);
				(*pos)--)
			;
		if (val == myNode->val[*pos]) {
			//printf("%d \n    ^ Megtalalva", val);
			return;
		}
	}
	search(val, pos, myNode->link[*pos]);

	return;
}

//BTREE ^^
//HASH ¡¡
struct hash_and_memo** tail(char *str, unsigned int len) {
	unsigned int this_hash_index;
	unsigned short place_in_table;
	struct hash_and_memo **ham_ptr;

	this_hash_index = rolling_hash_index(str, len);
	place_in_table = this_hash_index % legnagyobbSzam;
	for (ham_ptr = &head[place_in_table]; *ham_ptr; ham_ptr =
			&(*ham_ptr)->next) {

		if ((*ham_ptr)->stored_hash_index != this_hash_index)
			continue;

		if (strcmp(str, (*ham_ptr)->hash_mem) != 0) {
			continue;
		}

		break;
	}

	return ham_ptr;
}
//hashelés, minden karakter változtatja a hash értékét, sorrendtõl függõen, pl. ABA = 786418890 ; BAB = 2942192335 (minél nagyobb annál kisebb az esélye a hibának, és a hashok összeütközésének, valamint jobban elosztódnak  a táblában)
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

