#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MALLOC_FAIL -1
#define EMPTY_FILE -2



typedef struct el {
	int value; //vrijednost koju upisujemo
	struct el* left;//pokazivac na lijeve elemente
	struct el* right; //pokazivac na desne elemente
} _el;//struktura elemenata

typedef _el* element; //pointer na strukturu Element

element allocate(element);//funkcija za alociranje memorije novom elementu
element addNewElement(element, int);//fnkc za dodavanje novog el.
int inorder(element);//funkc za inorder ispis
int preorder(element);//funkc za preorder ispis
int postorder(element);//funkc za postorder ispis
int levelOrder(element);//funkc za level order ispis
element find(element, int);//funkcija za pronalazak odredenog elementa
element deleteElement(element, int);//funkcij za brisanje odredenog elementa
int freeTree(element);//oslobadanej memorije


int main() {

	element root = (element)malloc(sizeof(_el)); //inicijalizacija za pocetni root direktorij
	root->left = NULL;
	root->right = NULL;

	int a = 0;

	printf("Dodaj pocetnu vrijednost binarnog stabla:\n");
	scanf("%d", &a);
	root->value = a;

	int choice = 0, newVal = 0;

	do {
		printf("\n1-Unos novog elementa\n2-Ispis inorder\n3-Ispis preorder\n4-Ispis postorder\n5-Ispis level order\n6-Pronadi element\n7-Brisi element\n8-Exit\n\n");//izbor
		scanf("%d", &choice);
		switch (choice) {
		case 1:
			printf("Dodaj zeljenu vrijednost na binarno stablo:\n");
			scanf("%d", &newVal);
			addNewElement(root, newVal);
			break;
		case 2:
			inorder(root);
			break;
		case 3:
			preorder(root);
			break;
		case 4:
			postorder(root);
			break;
		case 5:
			levelOrder(root);
			break;
		case 6:
			printf("Dodaj vrijednost koju zelis naci na binarnom stablu:\n");
			scanf("%d", &newVal);
			find(root, newVal);
			break;
		case 7:
			printf("Dodaj vrijednost koju zelis izbrisati na binarnom stablu:\n");
			scanf("%d", &newVal);
			deleteElement(root, newVal);

			break;
		case 8:
			freeTree(root);
			printf("Exit.\n");
			break;

		default:
			printf("Pogresan unos.\n");
			break;
		}
	} while (choice != 8);

	return EXIT_SUCCESS;
}

element allocate(int val) {//alokacija
	element newEl = (element)malloc(sizeof(_el));

	newEl->left = NULL;
	newEl->right = NULL;
	newEl->value = val;
	return newEl;

}

element addNewElement(element head, int newValue) {

	if (head == NULL) {
		return allocate(newValue);//ako je na NULL znaci da smo dosli do kraja i mozemo smjestiti novi element
	}
	if (head->value > newValue)
		head->left = addNewElement(head->left, newValue);//ako je manja vrijednost znaci da ide ulijevo pa head micemo lijevo
	if (head->value < newValue)
		head->right = addNewElement(head->right, newValue);//ako je veca obrnuto

	return head;
}


int inorder(element head) {
	if (head == NULL)
		return EXIT_SUCCESS;

	inorder(head->left);//rekurzivno prolazi sve elemente nalijevo dok ne dode do NULL
	printf("%d ", head->value);//kad dode do NULL, petlja izlazi, vraca se na stari element, ispisuje ga
	inorder(head->right);//provjerava ima li element desno, ako ima taj elemment se sad provjerava kao novi head, idemo sve iznova
	return EXIT_SUCCESS;
}

int preorder(element head) {
	if (head == NULL)
		return EXIT_SUCCESS;

	printf("%d ", head->value);//petlja ispisuje head clan
	inorder(head->left);//rekurzivno prolazi sve elemente nalijevo dok ne dode do NULL, onda ispisuje
	inorder(head->right);//rekurzivno prolazi sve elemente nadesno dok ne dode do NULL, onda ispisuje
	return EXIT_SUCCESS;
}

int postorder(element head) {
	if (head == NULL)
		return EXIT_SUCCESS;

	inorder(head->left);//rekurzivno prolazi sve elemente nalijevo dok ne dode do NULL, onda provjerava elemente nadesno
	inorder(head->right);//rekurzivno prolazi sve elemente nadesno dok ne dode do NULL, onda ispisuje
	printf("%d ", head->value);//petlja ispisuje head clan
	return EXIT_SUCCESS;
}

int levelOrder(element head) {
	if (head == NULL)
		return EXIT_SUCCESS;

	element array[15];
	int comp = 0, counter = 0;//dvije vrijednosti:jedna broji clanove niza a druga se usporeduje s njom te upisuje nove elemente u niz

	array[counter++] = head; //prvi element ide na prvu poziciju u listi

	while (comp < counter) {//lista ce se vrtiti sve dok clanovi lijevo i desno postoje, ako prestanu postojati comp ce prestati kaskati za counterom
		element temp = array[comp++];//izvadi iz reda
		printf("%d ", temp->value);

		if (temp->left != NULL)
			array[counter++] = temp->left;//lijevi element se upisuje u red
		if (temp->right != NULL)
			array[counter++] = temp->right;//desni element se upisuje u red
	}

	return EXIT_SUCCESS;
}

element find(element head, int wantedValue)
{
	if (head == NULL) {
		printf("Vrijednost nije pronadena");
		return NULL; //u slucaju da nije proadena vrijednost
	}

	if (wantedValue == head->value) {
		printf("Vrijednost %d je pronadena", head->value);
		return head; //u slucaju da je pronadena vrijednost
	}
	if (wantedValue < head->value)
		head = find(head->left, wantedValue); //ide lijevo rekurzivno
	if (wantedValue > head->value)
		head = find(head->right, wantedValue); // desno rekurzivno


	return head;
}


element deleteElement(element head, int val) {
	if (head == NULL)
		return NULL;

	if (val < head->value)
		head->left = deleteElement(head->left, val);
	else if (val > head->value)
		head->right = deleteElement(head->right, val);
	else {//ako smo iscrpili sve opcIJE znaci da smo pronasli element koji trbamo izbrisati

		if (head->left == NULL) {
			element temp = head->right;//ako nema lijeve grane mozemo samo spremiti desnu u temp i osloboditi head
			free(head);
			return temp;
		}
		if (head->right == NULL) {//ako nema lijeve grane mozemo samo spremiti desnu u temp i osloboditi head
			element temp = head->left;
			free(head);
			return temp;
		}


		element temp = head->right;//ako ima i lijevu i desnu granu onda biramo koju granu cemo duplicirati
		head->value = temp->value; //postavimo vrijednost desne grane na vrijednost elementa koji brisemo
		head->right = deleteElement(head->right, temp->value); //brise element koji je postao duplikat, ide rekurzivno dok ne nade neki clan koji nema lijevu ili desnu granu
	}

	return head;
}

int freeTree(element head) {
	if (head == NULL) {
		printf("Uspijesno oslobodena memorija!\n");
		return EXIT_SUCCESS;
	}

	freeTree(head->left); //ciscenje stabla u postorderu
	freeTree(head->right);
	free(head);
	
	return EXIT_SUCCESS;
}
