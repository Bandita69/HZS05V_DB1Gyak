/*
 * TA_2.3.c
 *
 *	File masolasa egy masik fileba.
 *  Created on: 2021. szept. 21.
 *      Author: HZS05V
 */
#include <stdio.h>
#include <stdlib.h>


int main() {

	FILE *fp1P, *fp2P;
	int ch;
	fp1P = fopen("fp1.txt", "r");
	fp2P = fopen("fp2.txt", "w");
	if (!fp1P || !fp2P) {
		perror("Error opening input file: ");
		exit(1);
	}
	while ((ch = fgetc(fp1P)) != EOF)
		fputc(ch, fp2P);
	puts("File masolva");
	return (0);

}
