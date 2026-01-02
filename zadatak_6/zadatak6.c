#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MALLOC_FAIL -1
#define EMPTY_FILE -2

#define FIRST_DATE_BIGGER 1
#define SECOND_DATE_BIGGER -1
#define SAME_DATE 0

typedef struct el {
    struct el* next;
    char* product;
    int amount;
    float price;
} contents; //struktura za artikle

typedef struct da {
    int year;
    int month;
    int day;
} date;//struktura za datume

typedef struct elem {
    contents* child;
    struct elem* next;
    char* name;
    date* date_;
} reciept; //struktura za racune

typedef reciept* Recs;//pointeri za racune i artikle
typedef contents* Con;

int openReciepts(Recs head);//deklaracije funkcija; funkcija za otvaranje i sortiranje racuna
Recs assignReciept(char* filename);//funkcija za dodjeljivanje i alokaciju sadrazaja racuna
Con assignContent(char* line);//funkcija za za unos imena, kolicine i cijene sadrzaja
int sortReciepts(Recs head, Recs reciept);//funkcija za sortiranje racuna po datumu
int compareDates(date* d1, date* d2);//funkcija za usporedivanje datuma
int sortContents(Con* head);//funkcija za sortiranje abecedno
int readDate(date* dat1, date* dat2, char* product, Recs head);//funkcija za ciranje podataka izmedu dva datuma
int freeReciepts(Recs head);//funkcija za oslobaanje memorije


int main() {
    date dat1, dat2;
    char product[20];
    reciept head;
    head.child = NULL;
    head.next = NULL;
    head.name = NULL;
    head.date_ = NULL;

    if (openReciepts(&head) == EMPTY_FILE) {//provjera ako se moze otvoriti racuni.txt
        printf("Datoteka racuni.txt ne postoji.\n");
        return EXIT_FAILURE;
    }


    printf("Pocetni datum:\n");
    scanf("%d %d %d", &dat1.year, &dat1.month, &dat1.day);

    printf("Zavrsni datum:\n");
    scanf("%d %d %d", &dat2.year, &dat2.month, &dat2.day);


    printf("Unesite specificni artikl:\n");
    scanf("%s", product);

    readDate(&dat1, &dat2, product, &head);

    freeReciepts(&head);
    return EXIT_SUCCESS;
}





int readDate(date* dat1, date* dat2, char* product, Recs head) {//funkcija za citanje podataka izmedu dva datuma
    Recs p = head->next;
    int totalAmount = 0;
    float totalPrice = 0.0;

    while (p != NULL) {
        if (compareDates(p->date_, dat1) >= 0 && compareDates(p->date_, dat2) <= 0) {//usporeduje datume i ako su izmedu ulazi u uvijet, ako ne ide do sljedeceg do ne dode do nule
            Con c = p->child;
            while (c != NULL) {
                if (strcmp(c->product, product) == 0) {
                    totalAmount += c->amount;//obicni brojaci
                    totalPrice += c->amount * c->price;
                }
                c = c->next;//prolazi kroz sve contente dok ne dode do NULL
            }
        }
        p = p->next;
    }

    printf("%s: kolicina = %d, novac = %f\n", product, totalAmount, totalPrice);
    return 0;
}


int openReciepts(Recs head)//funkcija za otvaranje racuna
{
    char c[25];
    FILE* fp = fopen("racuni.txt", "r");
    if (fp == NULL)
        return EMPTY_FILE;

    while (fscanf(fp, "%s", c) == 1) {//cita jedan po jedan redak sve dok ne dode do EOF 
        Recs newReciept = assignReciept(c);//alokacija novog racuna
        sortReciepts(head, newReciept);//sortira automatski racune
    }
    Recs p = head->next;
    printf("Sortirani racuni:\n");
    while (p != NULL) {
        printf("%s\n", p->name);
        p = p->next;///provjera jesu li sortirani
    }

    fclose(fp);
    return EXIT_SUCCESS;
}

Recs assignReciept(char* recieptName) {//funkcija za alokaciju i prididavanje contenta racunima
    FILE* f = fopen(recieptName, "r");
    if (f == NULL) {
        printf("GRESKA 2");//provjera moze li se datoteka otvoriti
        exit(EMPTY_FILE);
    }
    char c[20];

    Recs newRec = (Recs)malloc(sizeof(reciept));//alociramo memoriju
    if (!newRec)
        exit(MALLOC_FAIL);//provjera malloc

    newRec->next = NULL;
    newRec->name = _strdup(recieptName);
    newRec->child = NULL;

    date* d = (date*)malloc(sizeof(date));//POInter za datum
    if (!d)
        exit(MALLOC_FAIL);

    if (fgets(c, sizeof(c), f)) {//if jer cita samo prvu liniju u kodu
        sscanf(c, "%d-%d-%d", &d->year, &d->month, &d->day);//dodjeljuje datum
    }
    newRec->date_ = d;


    while (fgets(c, sizeof(c), f)) {//while jer zelimo proci kroz ostatak datoteke
        c[strcspn(c, "\n")] = 0;//uzima \n na kraju svake linije

        if (newRec->child == NULL) {//ako je ovo prvi red u racunu njegov content postavlja kao dijete
            newRec->child = assignContent(c);
        }
        else {//prolazi ako nije prvi red 
            Con content = newRec->child;
            while (content->next != NULL)//postavlja na kraj sadrzaja 
                content = content->next;
            content->next = assignContent(c);
        }
    }

    sortContents(&newRec->child);

    fclose(f);
    return newRec;
}

Con assignContent(char* c) {//funkcija za unos imena, kolicine i cijene u vezanu listu
    Con newContent = (Con)malloc(sizeof(contents));
    if (!newContent)
        exit(MALLOC_FAIL);

    char buffer[20];
    newContent->next = NULL;

    sscanf(c, "%s %d %f", buffer, &newContent->amount, &newContent->price);//uzima ime, kolicinu i cijenu
    newContent->product = _strdup(buffer);

    return newContent;
}


int compareDates(date* d1, date* d2) {
    if (d1->year > d2->year)
        return FIRST_DATE_BIGGER;
    else if (d1->year < d2->year)
        return SECOND_DATE_BIGGER;//usporeduje godine


    if (d1->month > d2->month)//Usporeduje mjesece
        return FIRST_DATE_BIGGER;
    else if (d1->month < d2->month)
        return SECOND_DATE_BIGGER;

    if (d1->day > d2->day)
        return FIRST_DATE_BIGGER;
    else if (d1->day < d2->day)
        return SECOND_DATE_BIGGER;
    else
        return SAME_DATE;//usporeduje dane, ako je datum ranije ili kasnije vec je izasao iz petlje, ali ako je jednako nastavlja vrtiti sve do ovdje
}


int sortReciepts(Recs head, Recs reciept) {//sortira racune po datumu
    Recs p = head;
    while (p->next != NULL && compareDates(p->next->date_, reciept->date_) < 0) {//sve dok je drugi datum kasnije krece se kroz petlju
        p = p->next;
    }
    reciept->next = p->next;//kada pronade datum koji je jednak ili ranije od starog datuma mjenja im mjesta
    p->next = reciept;
    return EXIT_SUCCESS;
}



int sortContents(Con* head) {//sortira content po abecedi, pointer na pointer jer mijenja head
    Con sorted = NULL;//sortirani clan
    while (*head != sorted) {
        Con prev = NULL;//prethodni clan, postavlja ga na NULL jer head nije NULL
        Con current = *head;//sadasnji clan

        while (current->next != sorted) {//vrti sve dok se ne izjednaci current i sortirani clan
            Con nex = current->next;//sljedeci clan
            if (strcmp(current->product, nex->product) > 0) {//vrti ako je current po abecedi iza sljedeeg clana

                if (prev == NULL) {//ako smo na pocetku liste, onda head clan mjenjemo u sljedeci
                    *head = nex;
                }
                else {
                    prev->next = nex;//ako nismo na pocetku liste onda postavljamo da prev iza currenta
                }

                current->next = nex->next;
                nex->next = current;

                prev = nex;//nakon zamjene prev se pomjera na novi clan
            }
            else {//ako current nije iza sljedeceg clana, znaci daa je sortiran, pomakinjemo listu za jedan
                prev = current;
                current = current->next;
            }
        }

        sorted = current;//zadnji lement je sortiran i posaje nova granica
    }

    return EXIT_SUCCESS;
}


int freeReciepts(Recs head) {
    Recs p = head->next;
    while (p != NULL) {//sve dok ne dodemo do kraja liste
        Recs tempRec = p;
        p = p->next;

        Con c = tempRec->child;
        while (c != NULL) {//prvo oslobadamo content
            Con tempCon = c;
            c = c->next;
            free(tempCon->product);
            free(tempCon);
        }

        free(tempRec->name);
        free(tempRec->date_);
        free(tempRec);
    }
    head->next = NULL;
    return EXIT_SUCCESS;
}