#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MALLOC_FAIL -1
#define EMPTY -2
#define MAX_CHAR 15

typedef struct Person {
    char* name;
    char* surname;
    int year_of_birth;
    struct Person* next;
} person; //strukutra za nase osobe

typedef person* Position; //definiramo pointer na strukturu


int AddAtBeginning(const char*, const char*, int, Position*); //funkcija za dodavanje na pocetak liste
int AddAtEnd(const char*, const char*, int, Position); //funkcija za dodavanje na kraj
int PrintList(Position); //funkcija za ispis podataka
Position FindBySurname(Position, const char*); //funkcija za pronalazak osobe po prezimenu
int DeleteBySurname(Position*, const char*); //funkcija za brisanje osobe po prezimenu

int main() {
    char name[MAX_CHAR], surname[MAX_CHAR];
    int year_of_birth; //lokalne varijable koje koristimo za unos podataka
    int choice; //varijabla koju koristimo kako bi odabir u switch case bio funkcionalan


    person Head;
    Head.name = NULL;
    Head.surname = NULL;
    Head.year_of_birth = 0;
    Head.next = NULL; //head element liste koji je prazan

    do {
        printf("\n1-Dodaj na pocetak\n2-Ispis\n3-Dodaj na kraj\n4-Trazi po prezimenu\n5-Brisi po prezimenu\n0-Izlaz\n\n");
        scanf("%d", &choice);


        switch (choice) { //korisnikov izbor ulazi u switch case
        case 1:
            printf("Unesite ime, prezime i godinu rodjenja: \n");
            scanf("%s %s %d", name, surname, &year_of_birth);
            AddAtBeginning(name, surname, year_of_birth, &Head.next); //poziv funkcije za dodavanje osobe na pocetak liste
            break;
        case 2:
            PrintList(Head.next); //poziv funkcije za ispis liste
            break;
        case 3:
            printf("Unesite ime, prezime i godinu rodjenja: \n");
            scanf("%s %s %d", name, surname, &year_of_birth);
            AddAtEnd(name, surname, year_of_birth, &Head); //poziv funkcije za dodavanje osobe na kraj liste
            break;
        case 4: {
            printf("Unesite prezime: \n");
            scanf("%s", surname);
            Position track = FindBySurname(Head.next, surname); //pokazivac na osobu s trazenim prezimenom, poziv funkcije
            if (track) //ako nije NULL pronaden je
                printf("\n%s %s, %d\n", track->name, track->surname, track->year_of_birth);
            else
                printf("Prezime nije pronadjeno.\n");
            break;
        }
        case 5: {
            printf("Unesite prezime za brisanje: \n");
            scanf("%s", surname);
            if (DeleteBySurname(&Head.next, surname) == 0) //nakon sto pozovemo funkciju ako vrati 0 znaci da je uspjesno izbrisana osoba
                printf("Osoba izbrisana.\n");
            else
                printf("Prezime nije pronadjeno.\n");
            break;
        }
        case 0:
            printf("\nKraj programa\n");
            break;
        default:
            printf("Nepoznata opcija.\n");
        }

    } while (choice != 0); //petlja se izvršava dok korisnik ne izabere opciju za izlaz


    Position temp; //oslobadanje memorije
    while (Head.next) { //prolazi listom sve dok ne dode do NULL
        temp = Head.next;
        Head.next = temp->next;
        free(temp->name);
        free(temp->surname);
        free(temp);
    }

    return 0;
}

int AddAtBeginning(const char* newname, const char* newsurname, int newyears, Position* p) {
    Position new_person = (Position)malloc(sizeof(person)); //alokacija memorije za novu osobu
    if (!new_person) return MALLOC_FAIL; //ako malloc ne uspije prekidamo program

    new_person->name = _strdup(newname); //_strdup duplicira string da svaka osoba ima svoju kopiju, tj da promjenom jedne ne mijenjamo ostale
    new_person->surname = _strdup(newsurname);
    new_person->year_of_birth = newyears; //pridodavanje novih vrijednosti

    new_person->next = *p; //novi element pokazuje na ono što je prije pokazivao prvi
    *p = new_person; //pokazivač na početak sada pokazuje na novi element
    return 0;
}

int PrintList(Position p) {
    while (p) { //sve dok p nije null prolazi kroz listu
        printf("\n%s %s, %d\n", p->name, p->surname, p->year_of_birth);
        p = p->next; //postavlja p na sljedeci clan liste
    }
    return 0;
}

int AddAtEnd(const char* newname, const char* newsurname, int newyears, Position p) {
    while (p->next)
        p = p->next; //trazimo posljedni clan liste koji pokazuje na NULL
    AddAtBeginning(newname, newsurname, newyears, &(p->next)); //saljemo &(p-next) da novi element postane zadnji
    return 0;
}



Position FindBySurname(Position p, const char* surname_) {
    while (p) { //sve dok p nije null prolazi kroz listu
        if (strcmp(p->surname, surname_) == 0)
            return p; //ako su prezimena ista pronalazi p i izlazi iz while petlje
        p = p->next; //prelazi na sljedeci element
    }
    return NULL;
}

int DeleteBySurname(Position* p, const char* surname_) {
    Position* current = p; // pokazivač na pokazivač

    while (*current) { //vrti dok postoji clan
        if (strcmp((*current)->surname, surname_) == 0) {
            Position temp = *current;   // sprema clan koji brisemo
            *current = (*current)->next;    // preskoči taj clan u list
            free(temp->name);
            free(temp->surname);
            free(temp);
            return 0; // uspješno obrisano
        }
        current = &(*current)->next; // prelazi na sljedeći clan
    }

    return EMPTY; // ništa nije pronađeno
}

