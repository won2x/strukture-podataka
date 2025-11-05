#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MALLOC_FAIL -1
#define EMPTY_fILE -2

typedef struct _postfix {
    int number;
    struct _postfix* next;
}Postfix; //struktura podfix za broj u listi

typedef Postfix* Position; //pokazivac na strukturu

int push(Position, int); //dodaje na vrh stoga
int pop(Position); //uklanja element s vrha stoga i vraca njegovu vrijednost
int loadAndCalcFromFile(Position); //ucitava podatke iz datoteke i obavlja racun


int main() {
    Postfix head{ 0, NULL }; //inicijalizira head

    printf("Rezultat je %d", loadAndCalcFromFile(&head));

    return EXIT_SUCCESS;
}

int push(Position head, int _number) {
    Position newEl = (Position)malloc(sizeof(Postfix)); //alokacija memorije
    if (!newEl) {
        printf("Greska pri alokaciji memorije!\n");
        return MALLOC_FAIL;
    }
    newEl->number = _number; //dodaje vrijednost novom elementu
    newEl->next = head->next;
    head->next = newEl; //postavlja novi element na vrh stoga
    return EXIT_SUCCESS;
}

int pop(Position head) {
    Position temp = head->next; //pronalazi element na vrhu stoga
    if (temp == NULL)
        return EXIT_FAILURE; //ako ne postoji element greska
    int n = temp->number;  //sprema vrijednost elementa na vrhu stoga

    head->next = temp->next; //postavlja clan iza na vrh stoga

    free(temp);  //oslobadanje memorije

    return n; //vracca vrijednost starog vrha stoga
}


int loadAndCalcFromFile(Position head) {
    char c[15]; //inicijalizira buffer
    FILE* fp = fopen("postfix.txt", "r"); //otvara datoteku iz koje zelimo citati

    if (fp == 0) {
        printf("Datoteka je prazna!");

        return EMPTY_fILE; //ako je datoteka prazna vraca gresku
    }

    while (fscanf(fp, "%s", c) != EOF) { //sve dok ne dodemo do kraja datoteke vrti funkciju
        switch (c[0]) { //prolazi kroz slucajeve 


        case '+': // ako je u datoteci operator
        {
            int b = pop(head);
            int a = pop(head); //uzima vrijednosti zadnja dva clana(dva clana na vrhu stoga) u datoteci
            push(head, a + b);  //obavlja odredenu operaciju te rezultat stavlja na vrh stoga
            break;
        }

        case '-':
        {
            int b = pop(head);
            int a = pop(head);
            push(head, a - b);
            break;
        }
        case '*':
        {
            int b = pop(head);
            int a = pop(head);
            push(head, a * b);
            break;
        }
        case '/':
        {
            int b = pop(head);
            int a = pop(head);
            push(head, a / b);
            break;
        }


        default:

            push(head, atoi(c)); //ako je u datoteci broj stavlja ga na vrh stoga
            break;
        }

    }
    fclose(fp);
    return pop(head); //vraca jedinu vrijednost u stogu tj. krajnji rezultat 
}

