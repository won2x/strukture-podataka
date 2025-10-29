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
int DeleteBySurname(Position, const char*); //funkcija za brisanje osobe po prezimenu

int AddBehindElement(const char*, const char*, int, const char*, Position); //funkcija za dodavanje iza druge osobe po prezimenu
int AddInfrontElement(const char*, const char*, int, const char*, Position); //funkcija za dodavanje ispred druge osobe po prezimenu
int SortSurnames(Position); //funkcija za sortiranje prezimena
int AddToFile(Position);//funkcija za dodavanje osoba u datoteku
int LoadFromFile(Position);//funkcija za dodavanje osoba iz datoteke


int main() {
    char name[MAX_CHAR], surname[MAX_CHAR];
    int year_of_birth; //lokalne varijable koje koristimo za unos podataka
    int choice; //varijabla koju koristimo kako bi odabir u switch case bio funkcionalan


    person head;
    head.name = NULL;
    head.surname = NULL;
    head.year_of_birth = 0;
    head.next = NULL; //head element liste koji je prazan

    do {
        printf("\n1-Dodaj na pocetak\n2-Ispis\n3-Dodaj na kraj\n4-Trazi po prezimenu\n5-Brisi po prezimenu");
        printf("\n6-Dodaj iza osobe\n7-Dodaj ispred osobe\n8-Sortiraj listu po prezmenima\n9-Dodaj u datoteku\n10-Dodaj iz dadoteke\n0-Izlaz\n\n");
        scanf("%d", &choice);


        switch (choice) { //korisnikov izbor ulazi u switch case
        case 1:
            printf("Unesite ime, prezime i godinu rodjenja: \n");
            scanf("%s %s %d", name, surname, &year_of_birth);
            AddAtBeginning(name, surname, year_of_birth, &head.next); //poziv funkcije za dodavanje osobe na pocetak liste
            break;
        case 2:
            PrintList(head.next); //poziv funkcije za ispis liste
            break;
        case 3:
            printf("Unesite ime, prezime i godinu rodjenja: \n");
            scanf("%s %s %d", name, surname, &year_of_birth);
            AddAtEnd(name, surname, year_of_birth, &head); //poziv funkcije za dodavanje osobe na kraj liste
            break;
        case 4: {
            printf("Unesite prezime: \n");
            scanf("%s", surname);
            Position track = FindBySurname(head.next, surname); //pokazivac na osobu s trazenim prezimenom, poziv funkcije
            if (track) //ako nije NULL pronaden je
                printf("\n%s %s, %d\n\n", track->name, track->surname, track->year_of_birth);
            else
                printf("Prezime nije pronadjeno.\n\n");
            break;
        }
        case 5: {
            printf("Unesite prezime za brisanje: \n");
            scanf("%s", surname);
            if (DeleteBySurname(&head, surname) == 0) //nakon sto pozovemo funkciju ako vrati 0 znaci da je uspjesno izbrisana osoba
                printf("Osoba izbrisana.\n\n");
            else
                printf("Prezime nije pronadjeno.\n\n");
            break;
        }
        case 0:
            printf("\nKraj programa\n");
            break;

            //3. zadatak
        case 6: {
            char tracker[MAX_CHAR]; //prezime koje trazimo
            printf("Unesite prezime osobe iza koje zelite dodati novu osobu: \n");
            scanf("%s", tracker);
            printf("Unesite ime, prezime i godinu rodjenja: \n");
            scanf("%s %s %d", name, surname, &year_of_birth);

            if (AddBehindElement(name, surname, year_of_birth, tracker, head.next) == 0)//ako vrati 0 znaci da je pronadeno prezime,head.next saljemo jer nam prvi clan nije potreban
                printf("Osoba dodana iza %s.\n\n", tracker);
            else
                printf("Prezime nije pronadjeno.\n\n");
            break;
        }
        case 7: {
            char tracker[MAX_CHAR];
            printf("Unesite prezime osobe ispred koje zelite dodati novu osobu: \n");
            scanf("%s", tracker);
            printf("Unesite ime, prezime i godinu rodjenja: \n");
            scanf("%s %s %d", name, surname, &year_of_birth);

            if (AddInfrontElement(name, surname, year_of_birth, tracker, &head) == 0)//saljemo &head da nam ne preskoci prvi clan
                printf("Osoba dodana ispred %s.\n\n", tracker);
            else
                printf("Prezime nije pronadjeno.\n\n");
            break;
        }
        case 8:
            SortSurnames(&head); //poziv funkcije
            printf("Lista je azurirana.\n\n");
            break;
        case 9:
            AddToFile(&head);//poziv funkcije
            break;
        case 10:
            LoadFromFile(&head);//poziv funkcije
            break;
        default:
            printf("Nepoznata opcija.\n");
        }

    } while (choice != 0); //petlja se izvršava dok korisnik ne izabere opciju za izlaz


    Position temp; //oslobadanje memorije
    while (head.next) { //prolazi listom sve dok ne dode do NULL
        temp = head.next;
        head.next = temp->next;
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

int DeleteBySurname(Position p, const char* surname_) {

    Position prev = p; // pokazivač na pocetnu osobu
    Position current = p->next; // pokazivač na osobu

    while (current) { //vrti dok postoji clan
        if (strcmp(current->surname, surname_) == 0) {
            Position temp = current;   // sprema clan koji brisemo
            prev->next = current->next;    // preskoči taj clan u list
            free(temp->name);
            free(temp->surname);
            free(temp);
            return 0; // uspješno obrisano
        }
        prev = prev->next; // prelazi na sljedeći clan
        current = current->next; // prelazi na sljedeći clan
    }

    return EMPTY; // ništa nije pronađeno
}

//3. zadatak

int AddBehindElement(const char* newname, const char* newsurname, int newyears, const char* tracking_surname, Position p) {
    while (p) { //sve dok p nije null prolazi kroz listu
        if (strcmp(p->surname, tracking_surname) == 0) {
            Position new_person = (Position)malloc(sizeof(person)); //alokacija memorije za novu osobu
            if (!new_person) return MALLOC_FAIL; //ako malloc ne uspije prekidamo program

            new_person->name = _strdup(newname); //_strdup duplicira string da svaka osoba ima svoju kopiju, tj da promjenom jedne ne mijenjamo ostale
            new_person->surname = _strdup(newsurname);
            new_person->year_of_birth = newyears; //pridodavanje novih vrijednosti


            new_person->next = p->next;
            p->next = new_person;
            return 0;
        }
        p = p->next; //prelazi na sljedeci element
    }
    return EMPTY;

}

int AddInfrontElement(const char* newname, const char* newsurname, int newyears, const char* tracking_surname, Position p) {
    Position prev = p; //postavljamo prev na head
    Position current = p->next; //postavljamo current na prvi pravi element
    while (current) { //sve dok p nije null prolazi kroz listu
        if (strcmp(current->surname, tracking_surname) == 0) {
            Position new_person = (Position)malloc(sizeof(person)); //alokacija memorije za novu osobu
            if (!new_person) return MALLOC_FAIL; //ako malloc ne uspije prekidamo program

            new_person->name = _strdup(newname); //_strdup duplicira string da svaka osoba ima svoju kopiju, tj da promjenom jedne ne mijenjamo ostale
            new_person->surname = _strdup(newsurname);
            new_person->year_of_birth = newyears; //pridodavanje novih vrijednosti


            new_person->next = prev->next;
            prev->next = new_person;
            return 0;
        }
        prev = prev->next; //prelazi na sljedeci element
        current = current->next;
    }
    return EMPTY;
}

int SortSurnames(Position p) {
    if (!p->next || !p->next->next) return EMPTY;//ako nemamo clanova ili imamo samo jedan clan izlazi iz funkcije

    Position sorted = NULL;//inicijaliziramo sorted element

    while (p->next != sorted) {//vrti funkciju sve dok sljedeci clan nije sortiran
        Position prev = p;
        Position current = p->next;//postavljamo sadasnji i prijasnji clan

        while (current->next != sorted) {//vrti dok sljedeci clan nije sortiran(posljednji clan u listi)
            if (strcmp(current->surname, current->next->surname) > 0)//usporedujemo sadasnji clan i sljedeci clan
            {

                Position temp = NULL;
                temp = current->next;
                current->next = temp->next;
                temp->next = current;//bubble sort

                prev->next = temp;//prijasnji prev pokazuje na zamjenjeni clan


                prev = temp;//prev postavljamo na zamijenjeni clan
            }
            else {
                prev = current;
                current = current->next;//ako ne dode do zamijene mijenjamo pozicije prev i current
            }
        }
        sorted = current;//postavljamo sorted na zadnji sortirani clan u listi
    }
    return 0;
}

int AddToFile(Position p) {

    FILE* f = fopen("students.txt", "w");
    if (f == NULL) { //provjera mozemo li otvoriti datoteku uspjesno
        printf("Problem s otvaranjem datoteke");
        return EXIT_FAILURE;
    }
    Position q = p->next;//posto ne zelimo dodati head u datoteku krecemo od prvog pravog clana
    while (q) {
        fprintf(f, " %s %s, %d\n", q->name, q->surname, q->year_of_birth);//dodajemo ime prezime i godinu rodenja u datoteku
        q = q->next;
    }
    fclose(f);
    printf("Lista je pohranjena");
    return 0;
}

int LoadFromFile(Position p) {

    FILE* f = fopen("students.txt", "r");
    if (f == NULL) { //provjera mozemo li otvoriti datoteku uspjesno
        printf("Problem s otvaranjem datoteke");
        return EXIT_FAILURE;
    }

    char name[MAX_CHAR], surname[MAX_CHAR];
    int year_of_birth;
    while ((fscanf(f, "%s %s %d", name, surname, &year_of_birth)) == 3)//sve dok imamo 3 clana u datoteci vrti se funkcija
        AddAtEnd(name, surname, year_of_birth, p);//dodajemo na kraju liste
    fclose(f);
    printf("Lista je pohranjena");
    return 0;
}