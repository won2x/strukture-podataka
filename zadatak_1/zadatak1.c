#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#define MAX_POINTS 50

#define FILE_NOT_OPENED -1

typedef struct {
	char name[30];
	char surname[30];
	int points;

}student_info; //struktura za podatke o studentima

int CountStud(const char*); //funkcija koja broji koliko linija se nalazi u datoteci

int AddStudData(const char*, student_info*, int); //funkcija kojom unosimo podatke

int PrintStudData(student_info*, int); //funkcija za ispis podataka


int main() {

	int student_number = 0; //broj studenata

	student_number = CountStud("studenti.txt"); //varijabli pridruzujemo broj studenata pomocu funkcije

	printf("Broj zapisanih studenta je %d.\n", student_number);

	if (student_number <= 0)
		return 1; //ako nema studenata ili se datoteka ne moze otvoriti prekidamo program

	student_info* students = (student_info*)malloc(student_number * sizeof(student_info)); //alokacija memorije za sve studente

	if (students == NULL) {//provjeravamo je li alokacija uspjesna
		printf("Greska u alokaciji");
		return 1;
	}

	AddStudData("studenti.txt", students, student_number); //ucitavanje podataka u strukturu


	PrintStudData(students, student_number);//ispis podataka


	free(students);//oslobadanje memorije


	return 0;
}

int CountStud(const char* file)
{
	int stud = 0; //brojac studenata
	FILE* f = fopen(file, "r");
	if (f == NULL) { //provjerava moze li otvoriti datoteku uspjesno
		printf("Problem s otvaranjem datoteke");
		return -1;
	}
	char c;
	char lastChar = '\0';

	while ((c = fgetc(f)) != EOF) { // citamo znak po znak do kraja datoteke
		if (c == '\n') {
			stud++;  //dodajemo redak svaki put kad dodemo do '\n'
		}

		lastChar = c; //spremamo zadnji znak u datoteci



	}

	if (lastChar != '\n' && lastChar != '\0') { //provjeravamo je li zadnji znak u datoteci bio '\n' ili '\0', ako nije onda dodajemo brojacu jos jednu liniju
		stud++;
	}
	fclose(f);
	return stud;

}

int AddStudData(const char* file, student_info* students, int a) {
	FILE* f = fopen(file, "r");
	if (f == NULL) { //provjera mozemo li otvoriti datoteku uspjesno
		printf("Problem s otvaranjem datoteke");
		return -1;
	}

	for (int i = 0; i < a;i++) {
		fscanf(f, "%s %s %d", students[i].name, students[i].surname, &students[i].points); //učitavanje svakog studenta iz datoteke
	}
	fclose(f);
	return 0;
}


int PrintStudData(student_info* stud, int a) {
	if (stud == NULL) { //provjera je li pokazivac NULL
		printf("Problem s otvaranjem datoteke");
		return -1;
	}

	for (int i = 0;i < a;i++) {
		float relative = (float)stud[i].points / MAX_POINTS * 100;//odredujemo relativni broj bodova

		printf("%s %s %d %.2f%%\n", stud[i].name, stud[i].surname, stud[i].points, relative); //ispis podataka

	}

	return 0;

}