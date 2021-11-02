/*
 ============================================================================
 Name        : benchmark.c
 Author      : HZS05v
 Version     : 0.1
 Copyright   : Your copyright notice
 Description : A hash �s a b-fa keres�si gyorsas�g�nak m�r�se, a fa/t�bla fel�p�t�s�t belesz�molva.
 -in.txt fileb�l val� felt�lt�s (alapb�l 1-t�l 99999-ig sz�mok )
 -99998 sz�m keres�se ( a txt file els� sora )
 -kapott sebess�gek bir�l�sa (clock() / CLOCKS_PER_SEC), nagyban f�gg a processzor terhelt�s�gt�l.
 -n�lam a b-f�t hozta ki gyorsabbnak t�bbsz�r.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Hash-es dolgok--------------------------------------------------------------
struct hash_and_memo {
	struct hash_and_memo *next;
	unsigned stored_hash_index;
	char *hash_mem;
};

#define magic_Karp_number 500000

struct hash_and_memo *head[magic_Karp_number] = { NULL, };
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
void traversal(struct BTreeNode*);

int main() {

	double startTime;
	double endTime;
	double timeElapsedBTree;
	int keresettSzam;

	FILE *pFileIn;

	pFileIn = fopen("in.txt", "r");

	startTime = (double) clock() / CLOCKS_PER_SEC;
	//btree Benchmark eleje ^^
	//---------------------------
	//btree program:
	int ch;
	int magicnumber;

	//skip a line
	fscanf(pFileIn, "%d", &keresettSzam);
	while (!feof(pFileIn)) {
		fscanf(pFileIn, "%d", &magicnumber);
		insert(magicnumber);
	}

	printf("\nKeresett szam:\n");
	search(keresettSzam, &ch, root);
	fclose(pFileIn);
	printf("\n");
	printf("Btree^^ \n ------------------------------------------ \nHash��\n");

	//---------------------------
	//btree Benchmark vege��

	endTime = ((double) clock() / CLOCKS_PER_SEC);

	timeElapsedBTree = endTime - startTime;






	//Hash Benchmark eleje��--------------------
	startTime = (double) clock() / CLOCKS_PER_SEC;

	//Hash Program:
	double timeElapsedHash;
	char buffer[magic_Karp_number];
	struct hash_and_memo **prt;
	size_t len;

	//szukseg van egy in.txt nevu filera, az utols� sor v�g�n is legyen enter (\n)
	pFileIn = fopen("in.txt", "r");

	//hibakezel�s
	if (pFileIn == NULL)
		perror("Error opening input file: ");

	//Keresett sz�m az elso sorban ( a btree azt skippeli )
	printf("Keresett szam:");
	while (fgets(buffer, magic_Karp_number, pFileIn)) {
		len = strlen(buffer);
		prt = tail(buffer, len);
		if (*prt) { //tal�lt
			printf("\n%s    ^ Megtal�tlam\n", buffer);
		} else { /* nem tal�lt */

			//dinamikus mem�ria allok�ci�, lehet ettol lassu a hash kereso, nemtudom
			*prt = malloc(sizeof **prt);
			(*prt)->next = NULL;
			(*prt)->stored_hash_index = rolling_hash_index(buffer, len);
			(*prt)->hash_mem = malloc(1 + len);
			memcpy((*prt)->hash_mem, buffer, 1 + len);
		}
	}
	fclose(pFileIn);

	//---------------------------
	// Hash Benchmark vege��

	endTime = ((double) clock() / CLOCKS_PER_SEC);

	timeElapsedHash = endTime - startTime;

	printf("\n");
	fflush(stdout);
	if (timeElapsedBTree >= timeElapsedHash) {
		printf("Hash volt gyorsabb %f -val\n",
				timeElapsedBTree - timeElapsedHash);

	} else {
		printf("Btree volt gyorsabb %f -val\n",
				timeElapsedHash - timeElapsedBTree);
	}

	return 0;
}
//F�ggv�nyek:
//BTREE�� (alatta van a hash)

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
		//ha kisebb mint a limit akkor beilleszti, ha nagyobb akkor kett�v�lasztja
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

// level�rt�k kereses
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
			printf("%d \n    ^ Megtalalva", val);
			return;
		}
	}
	search(val, pos, myNode->link[*pos]);

	return;
}

// Traverse, vegig futas a leveleken, nagyon koltslegigenyes nagy f�knal ( tesztel�shez )
void traversal(struct BTreeNode *myNode) {
	int i;
	if (myNode) {
		for (i = 0; i < myNode->count; i++) {
			traversal(myNode->link[i]);
			printf("%d ", myNode->val[i + 1]);
		}
		traversal(myNode->link[i]);
	}
}

//BTREE ^^
//HASH ��
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
		//lehet hogy baroms�g, de az elk�pzel�s az hogy abban a ritka esetben amikor k�t hash index egyforma, de a tartalmuk k�l�nb�z� akkor ne t�rjen vissza rossz �rt�kkel
		if (strcmp(str, (*ham_ptr)->hash_mem) != 0) {
			continue;
		}

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

