#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MALLOC_FAIL -1
#define EMPTY_FILE -2

#define MAX 20
#define hashSpace 11

typedef struct el {

	char* city_name;
	int population;
	struct el* left;
	struct el* right;
}city;

typedef city* City;

typedef struct el1 {
	char* country_name;
	char* path;

	City child;
	struct el1* next;

}country;

typedef country* Country;

int openFile(Country*); //definicije funkcija
int sortCountries(Country);
int assignChild(Country);
City sortCitiesTree(City, City);
int printCountries(Country*);
int printCities(City);
int freeCities(City);
int freeCountries(Country*);
int searchPopulation(Country*, int, char*);
int searchPopCity(City, int);

int hashKey(char*);//hash kljuc


int main() {

	Country head[11] = {};//urmmmm lista (✿◠ᴗ◠)(✿◠ᴗ◠)(✿◠ᴗ◠)(✿◠ᴗ◠)

	int pop;
	char name[MAX];

	openFile(head);
	printCountries(head);
	printf("Unesite populaciju:\n");
	scanf("%d", &pop);
	printf("Unesite zeljenu drzavu:\n");
	scanf("%s", name);

	searchPopulation(head, pop, name);


	freeCountries(head);
	return EXIT_SUCCESS;
}

int openFile(Country* head) {

	FILE* fp = fopen("drzave.txt", "r");
	if (fp == NULL)
		return EMPTY_FILE;

	char c[MAX], f[MAX];

	while (fscanf(fp, "%s %s", c, f) == 2) {//cita sve dok postoje 2 stvke
		Country newCountry = (Country)malloc(sizeof(country));
		if (newCountry == NULL) {
			fclose(fp);
			return MALLOC_FAIL;
		}

		newCountry->country_name = _strdup(c);
		newCountry->path = _strdup(f);
		newCountry->child = NULL;

		assignChild(newCountry);

		int i = hashKey(c);//spremamo vrijednost koja izade iz hash kljuca ovdje (✿◠ᴗ◠)(✿◠ᴗ◠)(✿◠ᴗ◠)(✿◠ᴗ◠)

		Country curr = head[i];
		Country prev = NULL;

		while (curr && strcmp(curr->country_name, c) < 0) {//sortiranje (✿◠ᴗ◠)(✿◠ᴗ◠)(✿◠ᴗ◠)(✿◠ᴗ◠)
			prev = curr;
			curr = curr->next;
		}

		newCountry->next = curr;
		if (prev == NULL)
			head[i] = newCountry;
		else
			prev->next = newCountry;
	}


	fclose(fp);
	return EXIT_SUCCESS;

}


int assignChild(Country newCountry) {

	FILE* fp = fopen(newCountry->path, "r");
	if (fp == NULL)
		return EMPTY_FILE;

	char C[MAX];
	int population;

	while (fscanf(fp, "%s %d", C, &population) == 2) {
		City newCity = (City)malloc(sizeof(city));
		if (!newCity) {
			fclose(fp);
			return MALLOC_FAIL;
		}

		newCity->city_name = _strdup(C);
		newCity->population = population;
		newCity->left = NULL;
		newCity->right = NULL;

		if (newCountry->child == NULL)
			newCountry->child = newCity;
		else {
			newCountry->child = sortCitiesTree(newCountry->child, newCity);
		}
	}

	fclose(fp);
	return 0;
}


City sortCitiesTree(City newCountrychild, City newCity) {
	City q = newCity;
	City p = newCountrychild;

	if (p == NULL)
		return q;


	if (q->population < p->population)
		p->left = sortCitiesTree(p->left, q);
	else if (q->population > p->population)
		p->right = sortCitiesTree(p->right, q);
	else {
		if (strcmp(p->city_name, q->city_name) > 0)
			p->left = sortCitiesTree(p->left, q);
		else
			p->right = sortCitiesTree(p->right, q);
	}

	return p;
}

int printCountries(Country* head)
{
	for (int i = 0; i < 11; i++) {
		Country p = head[i];
		while (p != NULL) {//provjerava postoji li drzava na tom mjestu, ako postoji ispisuje (✿◠ᴗ◠)(✿◠ᴗ◠)(✿◠ᴗ◠)(✿◠ᴗ◠)
			printf("%s\nGradovi:\n", p->country_name);
			printCities(p->child);
			p = p->next;
		}
	}
	return EXIT_SUCCESS;
}


int printCities(City head) {

	if (head == NULL)
		return EXIT_SUCCESS;
	printCities(head->right);
	printCities(head->left);
	printf("%s %d\n", head->city_name, head->population);


	return EXIT_SUCCESS;

}

int freeCities(City root) {
	if (!root) return EXIT_SUCCESS;;

	freeCities(root->left);
	freeCities(root->right);

	free(root->city_name); // strdup
	free(root);
	return EXIT_SUCCESS;
}


int freeCountries(Country* head) {
	for (int i = 0; i < 11; i++) {
		Country p = head[i];
		while (p != NULL) {//sve dok postoji oslobada se lista(✿◠ᴗ◠)(✿◠ᴗ◠)(✿◠ᴗ◠)(✿◠ᴗ◠)
			Country next = p->next;
			freeCities(p->child);
			free(p->country_name);
			free(p->path);
			free(p);
			p = next;
		}
	}
	return EXIT_SUCCESS;
}


int searchPopulation(Country* head, int pop, char* name) {
	int index = hashKey(name);
	Country p = head[index];


	while (p != NULL && strcmp(name, p->country_name) != 0) {//ako p nije prazno pretrazit ce sve drzave s istim kljucem (✿◠ᴗ◠)(✿◠ᴗ◠)(✿◠ᴗ◠)
		p = p->next;
	}

	if (strcmp(name, p->country_name) == 0)
		searchPopCity(p->child, pop);
	return EXIT_SUCCESS;
}

int searchPopCity(City head, int pop) {
	if (head == NULL)
		return EXIT_SUCCESS;

	searchPopCity(head->left, pop);
	searchPopCity(head->right, pop);
	if (head->population > pop)
		printf("%s %d\n", head->city_name, head->population);
	return EXIT_SUCCESS;
}


int hashKey(char* countryName) {//funkcija za kljuc (✿◠ᴗ◠)(✿◠ᴗ◠)(✿◠ᴗ◠)(✿◠ᴗ◠)(✿◠ᴗ◠)
	int sum = 0;
	for (int i = 0; i < 5 && countryName[i] != '\0'; i++) {
		sum += (unsigned char)countryName[i];
	}
	return sum % 11;
}