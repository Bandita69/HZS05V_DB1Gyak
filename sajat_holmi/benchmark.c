/*
 ============================================================================
 Name        : benchmark.c
 Author      : HZS05v
 Version     :
 Copyright   : Your copyright notice
 Description :
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

#define magic_Karp_number 500

struct hash_and_memo *head[magic_Karp_number] = { NULL, };
struct hash_and_memo** tail(char *str, unsigned len);

static unsigned int rolling_hash_index(char *str, unsigned lenght);

//BTREE-s dolgok----------------------------------------------------------------
#define MAX 3
#define MIN 2

struct BTreeNode {
	int val[MAX + 1], count;
	struct BTreeNode *link[MAX + 1];
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
	double timeElapsedHash;
	FILE *pFileIn;
	char buffer[magic_Karp_number];
	pFileIn = fopen("in.txt", "r");
	startTime = (double) clock() / CLOCKS_PER_SEC;
	// Benchmark eleje^^
	//---------------------------
	//program:
	int ch;
	int magicnumber;
	//skip a line
	fscanf(pFileIn, "%d", &magicnumber);
	while (!feof(pFileIn)) {
		fscanf(pFileIn, "%d", &magicnumber);
		insert(magicnumber);
	}

	// ki iras
	//traversal(root);

	printf("\nKeresett szam:\n");
	search(99998, &ch, root);
	printf("\n");
	printf("Btree^^ \n ------------------------------------------ \nHash¡¡\n");

	//---------------------------
	// Benchmark vege¡¡

	//ez egy irto kicsi szam
	endTime = ((double) clock() / CLOCKS_PER_SEC);

	timeElapsedBTree = endTime - startTime;

	fclose(pFileIn);

	//Masik Benchmark--------------------
	startTime = (double) clock() / CLOCKS_PER_SEC;
	//Hash program kezdete:

	struct hash_and_memo **prt;
	size_t len;

	//szukseg van egy in.txt nevu filera, az utolsó sor végén is legyen enter (\n)
	pFileIn = fopen("in.txt", "r");

	//hibakezelés
	if (pFileIn == NULL)
		perror("Error opening input file: ");

	//Keresett szám az elso sorban ( a btree azt skippeli )
	printf("Keresett szam:");
	while (fgets(buffer, magic_Karp_number, pFileIn)) {
		len = strlen(buffer);
		prt = tail(buffer, len);
		if (*prt) { //talált
			printf("\n%s    ^ Megtalátlam\n", buffer);
		} else { /* nem talált */

			//printf("%s", buffer);

			//dinamikus memória allokáció, lehet ettol lassu a hash kereso, nemtudom
			*prt = malloc(sizeof **prt);
			(*prt)->next = NULL;
			(*prt)->stored_hash_index = rolling_hash_index(buffer, len);
			(*prt)->hash_mem = malloc(1 + len);
			memcpy((*prt)->hash_mem, buffer, 1 + len);
		}
	}

	//---------------------------
	// Masodik Benchmark vege¡¡

	endTime = ((double) clock() / CLOCKS_PER_SEC);

	timeElapsedHash = endTime - startTime;


	printf("\n");
	fflush(stdout);
	printf("Btree: %f\n", timeElapsedBTree);
	printf("Hash: %f\n", timeElapsedHash);


	return 0;
}

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

	if (pos > MIN)
		median = MIN + 1;
	else
		median = MIN;

	*newNode = (struct BTreeNode*) malloc(sizeof(struct BTreeNode));
	j = median + 1;
	while (j <= MAX) {
		(*newNode)->val[j - median] = node->val[j];
		(*newNode)->link[j - median] = node->link[j];
		j++;
	}
	node->count = median;
	(*newNode)->count = MAX - median;

	if (pos <= MIN) {
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
		if (node->count < MAX) {
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

// level kereses
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

// Traverse, vegig futas a leveleken, nagyon koltslegigenyes nagy faknal
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
//HASH ¡¡
struct hash_and_memo** tail(char *str, unsigned int len) {
	unsigned int this_hash_index;
	unsigned short place_in_table;
	struct hash_and_memo **ham_ptr;

	this_hash_index = rolling_hash_index(str, len);
	place_in_table = this_hash_index % magic_Karp_number;
	for (ham_ptr = &head[place_in_table]; *ham_ptr; ham_ptr =
			&(*ham_ptr)->next) {
		// megnézi hogy az adott helyen lévõ hash pointere létezõ hash cimre mutat e, ha igen akkor break-eli a loopot
		if ((*ham_ptr)->stored_hash_index != this_hash_index)
			continue;
		//lehet hogy baromság, de az elképzelés az hogy abban a ritka esetben amikor két hash index egyforma, de a tartalmuk különbözõ akkor ne térjen vissza rossz értékkel
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

