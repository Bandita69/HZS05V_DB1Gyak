/*
 ============================================================================
 Name        : binaris_szemelyfile_add_torol.c
 Author      : HZS05v
 Version     :
 Copyright   : Your copyright notice
 Description : Készítsen programot, mely fel tud vinni személyeket (azonosító és név) bináris
 fájlba. Készítsen függvényt a) új rekordot létrehozatalára,b) létezõ rekord
 törlésére c) létezõ rekord módosítására
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct EmployeeInformation {
	char Name[40];
	int ID;
	int Salary;
	int deleted; //ha  deleted!=0 akkor torolve lett
};

#define max_emp 3
struct EmployeeInformation Data[max_emp];

void teljes_record(struct EmployeeInformation, FILE*);

struct EmployeeInformation findEmployee(struct EmployeeInformation, FILE*);

void ChangeEmpoyee(struct EmployeeInformation, FILE*);
void DeleteEmpoyee(struct EmployeeInformation, FILE *);

int main() {

	FILE *f;
	struct EmployeeInformation Employee;

	f = fopen("szemely_records_bin", "wb+");
	if (!f) {
		perror("Error opening input file: ");
		return 1;

	}
	// binaris adatbazis feltoltese 3 adattal
	int idk[] = { 1000, 1001, 1002, };
	int salaries[] = { 150, 200, 145, };
	char names[max_emp][40] = { "Cindy-Jo Wheatgrazer", "Jerry-Roy Pigtrigger",
			"Doc-Boy Oatfarmer",

	};

	int j;

	for (j = 0; j < max_emp; j++) {
		//ide jonne egy scanf, de igy konyebb tesztelni
		Data[j].ID = idk[j];
		strcpy(Data[j].Name, names[j]);
		Data[j].Salary = salaries[j];
		Data[j].deleted = 0;
		fwrite(&Data[j], sizeof(struct EmployeeInformation), 1, f);
	}

	// itt az az elkepzeles hogy ha az fread vegig megy a recordocon akkor is ad vissza egy recordot ha nem talalt hozza Empoyee ID-t,
	// es igy ezt valtoztatja meg nem egy adott ember adatat
	struct EmployeeInformation end = { "endoffile", 999, 999, 1 };
	fwrite(&end, sizeof(struct EmployeeInformation), 1, f);

	fclose(f);



	f = fopen("szemely_records_bin", "rb+");
	if (!f) {
		perror("Error opening input file: ");
		return 1;

	}



	struct EmployeeInformation find_emp;
	ChangeEmpoyee(find_emp, f);

	DeleteEmpoyee(find_emp, f);

	rewind(f);

	teljes_record(Employee, f);
	fclose(f);

	return 1;
}

struct EmployeeInformation findEmployee(struct EmployeeInformation Employee,
		FILE *f) {

	char EmployeeID[5];
	int EmployeeID_int;

	printf("Employee ID: ");
	fflush( stdout);
	fgets(EmployeeID, 6, stdin);

	EmployeeID_int = atoi(EmployeeID);
	//szamjegyek szama : veszi a  10s logaritmusat a szam abszolutertekenek, lefelekerekiti es ad hozza egyet
	if ((floor(log10(abs(EmployeeID_int))) + 1) != 4) {
		perror("Nem 4 jegybol all az ID");

	}

	fseek(f, 0, SEEK_SET);
	while ((fread(&Employee, sizeof(struct EmployeeInformation), 1, f))
			&& (Employee.ID != EmployeeID_int) && Employee.deleted == 0)
		;
	if (Employee.ID == EmployeeID_int) {

		printf("Found: %d %s %d\n", Employee.ID, Employee.Name,
				Employee.Salary);
		fflush( stdout);
	} else {
		printf("%d not found!\n", EmployeeID_int);
		fflush(stdout);
	}

	return Employee;

}

void teljes_record(struct EmployeeInformation Employee, FILE *f) {
	printf("\n <Rekordok>\n");
	while ((fread(&Employee, sizeof(struct EmployeeInformation), 1, f))) {
		if (Employee.deleted == 0) {
			printf("%d %s %d\n", Employee.ID, Employee.Name, Employee.Salary);
		}

	}
}

void ChangeEmpoyee(struct EmployeeInformation find_emp, FILE *f) {

	printf("Salary megvaltoztatasa 999-re. ");
	struct EmployeeInformation *EmployeeMod = malloc(
			sizeof(struct EmployeeInformation));
	*EmployeeMod = findEmployee(find_emp, f);
	long currentPosition;
	currentPosition = ftell(f);
	fseek(f, currentPosition - sizeof(struct EmployeeInformation), SEEK_SET);

	EmployeeMod->Salary = 999;

	fwrite(EmployeeMod, sizeof(struct EmployeeInformation), 1, f);
	free(EmployeeMod);

}

void DeleteEmpoyee(struct EmployeeInformation find_emp, FILE *f) {

	printf("Employee torlese. ");
	struct EmployeeInformation *EmployeeMod = malloc(
			sizeof(struct EmployeeInformation));
	*EmployeeMod = findEmployee(find_emp, f);
	long currentPosition;
	currentPosition = ftell(f);
	fseek(f, currentPosition - sizeof(struct EmployeeInformation), SEEK_SET);

	EmployeeMod->deleted = 1;

	fwrite(EmployeeMod, sizeof(struct EmployeeInformation), 1, f);
	free(EmployeeMod);

}
