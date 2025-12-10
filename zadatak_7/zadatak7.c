#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MALLOC_FAIL -1
#define EMPTY_FILE -2



typedef struct el {
	char* name; //nniz, oznacava ime svakog elementa
	struct el* sibling;//lista na sibling elemente
	struct el* child; //lista na child elemente
	struct el* parent;//lista na roditelj elemente
} element;//struktura elemenata

typedef element* directory; //pointer na strukturu element

int md(directory); //funkcija za pravljenje novih poddirektorija
directory cd_dir(directory);//funkcija za prebacivanje u poddirektorij
directory cd(directory);//funkcija za prebacivanje u roditeljski direktorij
int dir(directory);//funkcija za ispisivanje direktorija i njegovih poddirektorija



int md(directory root) { //funkcija md za novi direktorij
	directory root_ = root; //kopiramo root

	char name[15];
	printf("Unesi ime direktorija: \n");
	scanf("%s", name);


	directory new_dir = (directory)malloc(sizeof(element));//novi direktorij


	new_dir->child = NULL;
	new_dir->sibling = NULL;
	new_dir->parent = root_;//alociramo roditelja ovog direktorija na root
	new_dir->name = _strdup(name);



	if (root_->child == NULL) {
		root_->child = new_dir;//ako roditelj nema druge djece postavljamo novi direktorij kao njegovo dijete

		return EXIT_SUCCESS;
	}
	else {
		directory temp = root->child;
		while (temp->sibling != NULL)//ako direktorij ima deugu djecu trezimo zadnjeg siblinga 
			temp = temp->sibling;
		temp->sibling = new_dir;//kada pronademo zadnjeg siblinga na njega dodajemo novi direktorij


	}
	return EXIT_SUCCESS;
}

directory cd_dir(directory root) {
	char target[20];
	printf("Unesite ime poddriektorija u koji se zelite prebaciti.\n");
	scanf("%s", target); //ime trazenog direktorija


	directory temp = root->child;


	while (temp != NULL && strcmp(target, temp->name)) {//vrti petlju dok ne nade trazeni direktorij ili do kraja poddirektorija
		temp = temp->sibling;
	}
	if (temp == NULL) {
		printf("Ne postoji trazeni podddirektorij.\n");
		return root;
	}
	return temp;//ako ga pronade postavlja u mainu na ovaj poddirektorij

}

directory cd(directory child_) {
	directory root = child_->parent;//postavlja direktorij na svog roditelja
	if (root != NULL) {
		printf("Direktorij %s\n", root->name);
		return root;
	}
	else {
		printf("Vec se nalazite u root direktoriju\n");//ako nema roditelja znaci da smo vec u rootu
		return child_;
	}

}

int dir(directory root) {
	printf("\nDirektorij:%s\nPoddirektoriji:\n", root->name);
	directory temp = root->child;
	if (temp == NULL)
		printf("Ne postoje\n");

	while (temp != NULL) { //prolazi kroz sve siblinge 
		printf("%s\n", temp->name);
		temp = temp->sibling;
	}
	return EXIT_SUCCESS;
}


int main() {

	directory root = (directory)malloc(sizeof(element)); //alokacija i inicijalizacija za pocetni root direktorij

	root->child = NULL;
	root->parent = NULL;
	root->sibling = NULL;
	root->name = _strdup(":C");

	directory current = root; //pokazivac na direktorij u kojem se nalazimo
	int choice = 0;

	do {
		printf("1-md\n2-cd dir\n3-cd\n4-dir\n5-exit\n\n");//izbor
		scanf("%d", &choice);
		switch (choice) {
		case 1:
			md(current);
			printf("\n\n");
			break;
		case 2:
			current = cd_dir(current);
			if (current != NULL)
				printf("Trenutni direktorij %s.\n\n", current->name);
			break;
		case 3:
			current = cd(current);
			printf("\n\n");
			break;
		case 4:
			dir(current);
			printf("\n\n");
			break;
		case 5:
			printf("Exit.\n");
			break;

		default:
			printf("Pogresan unos.\n");
			break;
		}
	} while (choice != 5);




	return EXIT_SUCCESS;
}