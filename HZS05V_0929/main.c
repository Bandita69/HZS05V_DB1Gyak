/*
 ============================================================================
 Name        : TAFileOlvas.c   // at kell majd irni javaba
 Author      : HZS05v
 Version     : 0.1
 Copyright   : Your copyright notice
 Description : Írjon programot, amely egész típusú adatokat beolvassa a ,
kiszámítja az adatok összegét és kiírja a konzolra! Osztály neve: XYFileOlvas
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct SzamokStrukt
{
    int number;
};

#define max_szamok 3
struct SzamokStrukt Data[max_szamok];

void teljes_record(struct SzamokStrukt, FILE*);
int szam_szumma(struct SzamokStrukt, FILE *);
int adatok_szama(struct SzamokStrukt, FILE *);


int main()
{

    FILE *f;
    struct SzamokStrukt Szamocskak;

    f = fopen("vezeteknev.txt", "w+");
    if (!f)
    {
        perror("Error opening input file: ");
        return 1;

    }
    //  adatbazis feltoltese 3 adattal
    int numbers[] = { 10, 20, 30, };

    int j;

    for (j = 0; j < max_szamok; j++)
    {

        Data[j].number = numbers[j];
        fwrite(&Data[j], sizeof(struct SzamokStrukt), 1, f);
    }


    fclose(f);


    //Ujra megnyit

    f = fopen("vezeteknev.txt", "r");
    if (!f)
    {
        perror("Error opening input file: ");
        return 1;

    }

    int adat_szam = 0;
    adat_szam=adatok_szama(Szamocskak, f);
    printf("Adatok szama: %d\n", adat_szam);


    rewind(f);
    teljes_record(Szamocskak, f);
    rewind(f);


    int summa = 0;
    summa = szam_szumma(Szamocskak, f);
    printf("Adatok összege: %d\n", summa);
    fclose(f);

    return 1;
}

int szam_szumma(struct SzamokStrukt szam_stukt, FILE *f)
{

    int sum = 0;

    fseek(f, 0, SEEK_SET);
    while ((fread(&szam_stukt, sizeof(struct SzamokStrukt), 1, f)))
    {


        sum += szam_stukt.number;


    }

    return sum;

}

void teljes_record(struct SzamokStrukt szam_stukt, FILE *f)
{
    int i = 0;
    while ((fread(&szam_stukt, sizeof(struct SzamokStrukt), 1, f)))
    {

        printf("%d Adat : %d\n", i, szam_stukt.number);
        i++;


    }
}

int adatok_szama(struct SzamokStrukt szam_stukt, FILE *f)
{

    int i=0;
    while ((fread(&szam_stukt, sizeof(struct SzamokStrukt), 1, f)))
    {
        i++;
    }
    return i;

}

