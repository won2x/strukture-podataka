#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MALLOC_FAIL -1
#define EMPTY_FILE -2

#define MAX 20

typedef struct el {

	char* city_name;
	int population;
	struct el* next;
	struct el* left;
	struct el* right;
}city;

typedef city* City;

typedef struct el1 {
	char* country_name;
	char* path;

	City child;
	struct el1* next;
	struct el1* left;
	struct el1* right;

}country;

typedef country* Country;

int openFile(Country); //definicije funkcija
int sortCountries(Country);
int assignChild(Country);
City sortCitiesTree(City, City);
int printCountries(Country);
int printCities(City);
int freeCities(City);
int freeCountries(Country);
int searchPopulation(Country, int, char*);
int searchPopCity(City, int);

int openFileb(Country);
Country sortCountriesb(Country, Country);
int assignChildB(Country);
int sortCitiesList(City);
int freeCountriesb(Country);
int freeCitiesList(City);
int printCountriesb(Country);

int main() {

	country head;
	head.country_name = NULL;
	head.path = NULL;
	head.child = NULL;
	head.left = NULL;
	head.right = NULL;
	head.next = NULL;

	int pop;
	char name[MAX];

	openFile(&head);
	printCountries(&head);
	printf("Unesite populaciju:\n");
	scanf("%d", &pop);
	printf("Unesite zeljenu drzavu:\n");
	scanf("%s", name);

	searchPopulation(&head, pop, name);

	country headb;
	headb.country_name = NULL;
	headb.path = NULL;
	headb.child = NULL;
	headb.left = NULL;
	headb.right = NULL;
	headb.next = NULL;
	openFileb(&headb);

	printCountriesb(headb.left);

	freeCountries(&head);
	freeCountriesb(headb.left);
	return EXIT_SUCCESS;
}

int openFile(Country head) {

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
		newCountry->left = NULL;
		newCountry->right = NULL;

		assignChild(newCountry);


		newCountry->next = head->next;
		head->next = newCountry;
	}
	sortCountries(head);


	fclose(fp);
	return EXIT_SUCCESS;

}


int sortCountries(Country head) {
	Country curr = head->next;
	head->next = NULL; //prvi pravi clan

	while (curr != NULL) {
		Country next = curr->next; //spremamo sljedeci clan da bismo mogli raditi usporedbiu
		Country prev = head;//prev postavljamo na head

		while (prev->next != NULL && strcmp(prev->next->country_name, curr->country_name) < 0) {//vrti sve dok ne pronade clan kasnije po abecedi
			prev = prev->next;
		}

		curr->next = prev->next;//posto je pronasao kasniji clan ide zamjena
		prev->next = curr;

		curr = next; //krece se dalje
	}

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
		newCity->next = NULL;

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

int printCountries(Country head) {
	Country p = head->next;
	while (p != NULL) {
		printf("%s\nGradovi:\n", p->country_name);
		printCities(p->child);
		p = p->next;
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


int freeCountries(Country head) {
	Country temp = head->next; 
	Country next;

	while (temp) {
		next = temp->next;

		free(temp->country_name);
		free(temp->path);

		freeCities(temp->child);

		free(temp);
		temp = next;
	}
	return EXIT_SUCCESS;
}


int searchPopulation(Country head, int pop, char* name) {
	Country p = head->next;

	while (p != NULL && strcmp(name, p->country_name) != 0) {
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


//b

int openFileb(Country head) {

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
		newCountry->left = NULL;
		newCountry->right = NULL;

		assignChildB(newCountry);

		if (head->left == NULL)
			head->left = newCountry;
		else
			sortCountriesb(head->left, newCountry);

	}



	fclose(fp);
	return EXIT_SUCCESS;

}


Country sortCountriesb(Country head, Country newCountry) {
	if (head == NULL)
		return newCountry;

	if (strcmp(newCountry->country_name, head->country_name) < 0)
		head->left = sortCountriesb(head->left, newCountry);
	else
		head->right = sortCountriesb(head->right, newCountry);
	return head;
}

int assignChildB(Country newCountry) {

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
		newCity->next = NULL;

		if (newCountry->child == NULL)
			newCountry->child = newCity;
		else {
			newCity->next = newCountry->child;
			newCountry->child = newCity;
		}
	}
	sortCitiesList(newCountry->child);

	fclose(fp);
	return 0;
}

int sortCitiesList(City head)
{
	if (head == NULL || head->next == NULL)
		return EXIT_SUCCESS;

	City curr = head->next;
	head->next = NULL;

	while (curr != NULL) {
		City next = curr->next;
		City prev = head;

		while (
			prev->next != NULL &&
			(
				prev->next->population < curr->population ||
				(prev->next->population == curr->population &&
					strcmp(prev->next->city_name, curr->city_name) < 0)
				)
			) {
			prev = prev->next;
		}

		curr->next = prev->next;
		prev->next = curr;

		curr = next;
	}

	return EXIT_SUCCESS;
}


int printCountriesb(Country head)
{
	if (head == NULL)
		return EXIT_SUCCESS;

	printCountriesb(head->left);

	printf("%s\nGradovi:\n", head->country_name);
	City p = head->child;
	while (p) {
		printf("%s %d\n", p->city_name, p->population);
		p = p->next;
	}

	printCountriesb(head->right);

	return EXIT_SUCCESS;
}

int freeCountriesb(Country root)
{
	if (root == NULL)
		return EXIT_SUCCESS;

	freeCountriesb(root->left);
	freeCountriesb(root->right);

	freeCitiesList(root->child);
	free(root->country_name);
	free(root->path);
	free(root);

	return EXIT_SUCCESS;
}


int freeCitiesList(City head)
{
	while (head) {
		City temp = head;
		head = head->next;
		free(temp->city_name);
		free(temp);
	}
	return EXIT_SUCCESS;
}
