#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAXIME 50

typedef struct {
    int* karte;
    int velicina;
    int kapacitet;
} HAND;

typedef struct {
    char ime[MAXIME];
    float balance;
    int wins;
    int losses;
} Player;

extern int ukupnoDilaneKarte; //extern use, nista drugo

void mainMenu(Player* player);
int start(Player* player);
void pravila();
int vrijednostRuke(int ruka[], int brojKarata);
void ispisiRuku(int ruka[], int brojKarata);
HAND* izradiHand();
void dodajKartu(HAND* hand, int card);
void freeHand(HAND* hand);
void ispisiStatistiku(Player* player);
void zavrsiIgru(Player* player, HAND* rukaDiler, HAND* rukaIgrac, float ulog, int bodoviIgrac, int bodoviDiler);
void errPoruka(const char* message);
int comparePlayers(const void* a, const void* b);
void sortAndSearchStatistics();
void trimWhitespace(char* str);
void bankrot(Player* player);
void removeStats();


