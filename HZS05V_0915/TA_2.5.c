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

struct EmployeeInformation {
	char *Name;
	int ID;
	int Salary;
	int deleted; //ha  deleted!=0 akkor torolve lett
};

void teljes_record(struct EmployeeInformation Employee, FILE *f) {
	while ((fread(&Employee, sizeof(struct EmployeeInformation), 1, f))) {
		if (Employee.deleted == 0) {
			printf("%d %s %d\n", Employee.ID, Employee.Name, Employee.Salary);
		}

	}
}

struct EmployeeInformation* findEmployee(struct EmployeeInformation Employee,
		FILE *fd) {

	char EmployeeID[5];
	int EmployeeID_int;
	struct EmployeeInformation *talalt;

	printf("Employee ID: ");
	fflush( stdout);
	fgets(EmployeeID, 5, stdin);

	EmployeeID_int = atoi(EmployeeID);

	fseek(fd, 0, SEEK_SET);
	while ((fread(&Employee, sizeof(struct EmployeeInformation), 1, fd))
			&& (Employee.ID != EmployeeID_int) && Employee.deleted == 0)
		;
	if (Employee.ID == EmployeeID_int) {
		talalt = malloc(sizeof(Employee));
		talalt->ID = Employee.ID;
		talalt->Name = Employee.Name;
		talalt->Salary = Employee.Salary;
		printf("%d %s %d\n", Employee.ID, Employee.Name, Employee.Salary);
		fflush( stdout);
	} else {
		printf("Not found!");
		fflush(stdout);
	}

	return talalt;

}

int main() {

	FILE *f;
	struct EmployeeInformation Employee;
	struct EmployeeInformation *EmployeeMod;

	f = fopen("szemely_records_bin", "wb+");
	if (!f) {
		perror("Error opening input file: ");
		return 1;

	}
	int idk[] = { 1000, 1001, };
	int salaries[] = { 150, 200, };
	char names[3][40] = { "Super Chad", "Virigin Nomen",

	};

	int j;
	for (j = 0; j <= 1; j++) {
		//ide kell egy scanf
		Employee.ID = idk[j];
		Employee.Name = names[j];
		Employee.Salary = salaries[j];
		Employee.deleted = 0;
		fwrite(&Employee, sizeof(struct EmployeeInformation), 1, f);
	}
	fclose(f);

	f = fopen("szemely_records_bin", "rb+");
	if (!f) {
		perror("Error opening input file: ");
		return 1;

	}

	//Melyik Employee-t valtoztassuk meg?
	EmployeeMod = findEmployee(Employee, f);
	long currentPosition;
	currentPosition = ftell(f);
	//printf("position %ld\n", currentPosition);

	// a structura elejere visszi a byte pointeret
	fseek(f, currentPosition - sizeof(Employee), SEEK_SET);

	EmployeeMod->Salary = 100;

	//ezzel lehet 'torolni'
	EmployeeMod->deleted = 1;

	fwrite(EmployeeMod, sizeof(Employee), 1, f);
	fflush(f);
	rewind(f);
	teljes_record(Employee, f);
	fclose(f);

	return 1;
}
