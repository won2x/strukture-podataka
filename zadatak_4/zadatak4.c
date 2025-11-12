#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MALLOC_FAIL -1
#define EMPTY_FILE -2



typedef struct _polynomial {
	int coef;
	int exp;
	_polynomial* next;

}poly; //struktura za polinome

typedef poly* Position;

int load(Position, Position); //funkcija za ucitavanje datoteke 
int pushAndSort(Position, int, int); //funkcija za dodavanje na vrh pa sortiranje dodanih brojeva
int addPolys(Position, Position, Position); //funkcija za zbrajanje polinoma
int multiplyPolys(Position, Position, Position); //funkcija za mnozenje polinoma
int freeList(Position); //oslobadanje memorije


int main() {
	poly head = { 0,0,NULL };
	poly firstP = head;
	poly secondP = head;
	poly res1 = head;
	poly res2 = head;

	load(&firstP, &secondP); //poziva ucitavanje prvog i drugog polinoma

	addPolys(&res1, &firstP, &secondP); //zbrajanje

	multiplyPolys(&res2, &firstP, &secondP); //mnozenje

	freeList(&head);
	freeList(&firstP);
	freeList(&secondP);
	freeList(&res1);
	freeList(&res2);//oslobadanje memorije svih lista


	return EXIT_SUCCESS;
}

int load(Position first, Position second) {
	int coef, exp;
	int buffer;
	FILE* fp = fopen("polinom.txt", "r");
	if (fp == 0) {
		printf("Datoteka se ne moze otvoriti!");

		return EMPTY_FILE; //ako je datoteka prazna vraca gresku
	}
	while (fscanf(fp, "%d %d", &coef, &exp) == 2) { //prolzi kroz datoteku sve dok moze procitati 2 vrijednosti

		pushAndSort(first, coef, exp);  //salje vrijednosti koje je procitao u pushandsort


		buffer = fgetc(fp); //cita sljedeci znak u datoteci
		if (buffer == '\n') //ako je taj znak \n to je novi red(novi polinom) izlazi iz while petlje
			break;
		ungetc(buffer, fp); //ako nije vraca znak i nastavlja s citanjem

	}


	while (fscanf(fp, "%d %d", &coef, &exp) == 2) {//prolzi kroz datoteku sve dok moze procitati 2 vrijednosti
		pushAndSort(second, coef, exp); //salje vrijednosti koje je procitao u pushandsort


		buffer = fgetc(fp);//cita sljedeci znak u datoteci
		if (buffer == EOF) //ako je taj znak EOF to je kraj datoteke, izlazi iz while petlje
			break;
		ungetc(buffer, fp); //ako nije vraca znak i nastavlja s citanjem

	}


	fclose(fp);
	return EXIT_SUCCESS;
}



int pushAndSort(Position head, int _coef, int _exp) {


	Position previous = head;
	Position current = head->next; //postavlja previous na head, a current na prvi pravi clan


	while (current != NULL && current->exp > _exp) { //provjerava postoje li prijasnji clanovi i ako postoje je li njihov eksponent veci od eksponenta novog elementa, ovime provjeravamo i gdje cemo staviti novi clan u listi
		previous = current;
		current = current->next;//ako eksopnent novog elementa nije veci od eksponenta pravog clana, previous i current se pomakinju za jedno mjesto i opet ide provjera sve dok ne dode do kraja
	}

	if (current != NULL && current->exp == _exp) { //ako su eksponenti jednki dodaje ga vec na postojeci element
		current->coef += _coef; // zbroji koeficijente

	}
	else {  //ako ne postoji clan s jedniakim eksponentom dodajemo novi clan sortirano
		Position newEl = (Position)malloc(sizeof(poly)); //alokacija memorije
		if (!newEl) {
			printf("Malloc fail");
			return MALLOC_FAIL;
		}
		newEl->coef = _coef;
		newEl->exp = _exp;
		newEl->next = current;
		previous->next = newEl;//postavlja vrijednosti novog elementa, a newEl next na NULL 
	}

	return EXIT_SUCCESS;
}

int addPolys(Position head, Position firPoly, Position secPoly) {


	Position tempFirst = firPoly->next;
	Position tempSecond = secPoly->next; //posto smo poslali head od oba polinoma postavljamo njihove kopije na prve prave clanove

	while (tempFirst != NULL && tempSecond != NULL) { //vrti sve dok ne dodemo do kraja 
		if (tempFirst->exp == tempSecond->exp) { //ako su eksponenti jednaki zbraja clanove
			pushAndSort(head, tempFirst->coef + tempSecond->coef, tempFirst->exp);
			tempFirst = tempFirst->next;
			tempSecond = tempSecond->next; //prije izlazapomjera se u polinomu

		}
		else if (tempFirst->exp > tempSecond->exp) { //ako je eksponent prvog veci od eksponenta drugog prvi se samo prepisuje u novoj listi
			pushAndSort(head, tempFirst->coef, tempFirst->exp);
			tempFirst = tempFirst->next;//pomjeramo prvi na sljedeci clan

		}
		else {
			pushAndSort(head, tempSecond->coef, tempSecond->exp);
			tempSecond = tempSecond->next;//ako je drugi veci prepisujemo drugi i pomjeramo se u listi

		}
	}
	while (tempFirst != NULL) { //u slucaju da smo s drugim dosli do kraja a s prvim jos nismo prepisujemo preostale clanove prvog
		pushAndSort(head, tempFirst->coef, tempFirst->exp);
		tempFirst = tempFirst->next;
	}

	while (tempSecond != NULL) { //u slucaju da smo s prvim dosli do kraja polinoma a s drugim nismo prepisujemo clanove drugog
		pushAndSort(head, tempSecond->coef, tempSecond->exp);
		tempSecond = tempSecond->next;
	}



	Position current = head->next; //ispis, krece se od prvog pravog do NULL
	while (current != NULL) {
		printf("%dx^%d ", current->coef, current->exp);
		current = current->next;
	}
	printf("\n");

	return EXIT_SUCCESS;

}

int multiplyPolys(Position head, Position firPoly, Position secPoly) {

	Position tempFirst = firPoly->next; //kopije prvih pravih clanova
	Position tempSecond; //ovaj clan ceemo postavljati na prvi pravi svaki put u while petlji

	while (tempFirst != NULL) { //krece se po petlji sve dok ne dodemo do kraja prvog polinoma

		tempSecond = secPoly->next; //postavljamo tempSecond na prvi clan u drugom polinomu

		while (tempSecond != NULL) { //krece se po petlji sve dok ne dodemo do kraja drugog polinoma
			pushAndSort(head, tempFirst->coef * tempSecond->coef, tempFirst->exp + tempSecond->exp); //mnozi svaki sa svakim

			tempSecond = tempSecond->next; //postavljamo tempSecond na sljedeci clan tako da mozemo mnoziti svaki sa svakim


		}
		tempFirst = tempFirst->next; //sljedeci clan u prvom polinomu

	}



	Position current = head->next; //ispis,krece se od prvog pravog do NULL
	while (current != NULL) {
		printf("%dx^%d ", current->coef, current->exp);
		current = current->next;
	}
	printf("\n");

	return EXIT_SUCCESS;

}

int freeList(Position head) { //funkcija za oslobadanje memorije, posto smo imali 3 liste lakse ih je osloboditi funkcijom
	Position temp;
	Position current = head->next;
	while (current != NULL) {
		temp = current;
		current = current->next;
		free(temp); //sve dok ne dodemo do head->next==NULL oslobada cvorove
	}

	head->next = NULL;

	return EXIT_SUCCESS;
}