#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define EMPTY_FILE -2

typedef struct el {
	struct el* left;
	struct el* right;
	int value;
}tree;//struktura za binarno stablo

typedef struct el* Position;//pointer na stablo

Position insert(Position, int);//funkcija insert
int replace(Position);//funkcija replace
int freeTree(Position);//funkcija za oslobadanje stabla
int inorder(Position, FILE*);//funkcija za ispis inorder u datoteku

int main() {
	int array[10] = { 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 };//zadani niz
	Position head = NULL;//pointer koji koristimo

	for (int i = 0;i < 10;i++) {
		head = insert(head, array[i]);//poziv insert funkcije
	}
	replace(head);//poziv replace funkcije

	freeTree(head);//oslobadanje memorije

	srand(time(NULL));//koristenje funkcije srand

	int arr2[10]; //2. niz u koji cemo spremiti random brojeve
	for (int i = 0;i < 10;i++) {
		arr2[i] = (rand() % (90 - 10 + 1)) + 10;//koristenje funkcije rand()
	}

	head = NULL;//postavljanje head na NULL 

	for (int i = 0;i < 10;i++) {
		head = insert(head, arr2[i]);
	}

	FILE* fp = fopen("inorder.txt", "w");//otvaranje i provjera moze li se datoteka otvoriti
	if (fp == NULL)
	{
		printf("Greska u otvaranju");
		return EMPTY_FILE;
	}
	fprintf(fp, "Inorder random brojevi\n");
	inorder(head, fp);//pozivanje inorder ispisa

	replace(head);
	fprintf(fp, "\nInorder random brojevi replace\n");
	inorder(head, fp);//pozivanje inorder ispisa random brojeva

	freeTree(head);
	fclose(fp);
	return EXIT_SUCCESS;
}

Position insert(Position head, int val) {//funkcija insert

	if (head == NULL) {//ulazi tek kada smo dosli do NULL
		head = (Position)malloc(sizeof(tree));//alokacija
		head->value = val;
		head->left = NULL;
		head->right = NULL;
		return head;
	}
	if (head->value > val)//ako je vrijednost od heada veca stavi novu granu lijevo
		head->left = insert(head->left, val);
	else  //ako je vrijednost od heada manja stavi novu granu desno
		head->right = insert(head->right, val);
	return head;
}

int replace(Position head) {///funkcija za replace
	if (head == NULL)
		return 0;//vraca 0 jer na tom mjestu nema vrijednosti koja se moze zbrojiti

	int oldValue = head->value;//sprema staru vrijednost
	head->value = replace(head->left) + replace(head->right);//zbraja i postavlja vrijednosti u lijevoj i vrijednosti u desnoj grani
	return head->value + oldValue; //nova+stara vrijednost jer ne smijemo izgubiti staru vrijednost

}

int freeTree(Position head) {//oslobadanje memorije
	if (head == NULL)
		return EXIT_SUCCESS;//izlazi iz funkcije tek kad dode do NULL(kraja)

	freeTree(head->left);
	freeTree(head->right);
	free(head);
	return EXIT_SUCCESS;
}

int inorder(Position head, FILE* fp) {
	if (head == NULL)
		return EXIT_SUCCESS;

	inorder(head->left, fp);//rekurzivno prolazi sve elemente nalijevo dok ne dode do NULL
	fprintf(fp, "%d ", head->value);//kad dode do NULL, petlja izlazi, vraca se na stari element, ispisuje ga
	inorder(head->right, fp);//provjerava ima li element desno, ako ima taj elemment se sad provjerava kao novi head, idemo sve iznova

	return EXIT_SUCCESS;

}