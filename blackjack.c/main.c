#define _CRT_SECURE_NO_WARNINGS
#include "zaglavlje.h"

int main(void) {
    Player player = { "", 500.0, 0, 0 };
    mainMenu(&player);
    return 0;
}

void mainMenu(Player* player) {
    int izbor;
    char izaci;
    int validInput;

    while (1) {
        printf("\t\tDobrodosli u BLACKJACK!!!\n");
        printf("____________________________________________________________________\n");
        printf("1. Zaigraj!\n");
        printf("2. Procitaj pravila igre.\n");
        printf("3. Sortiraj i pretrazi statistiku.\n");
        printf("4. Izlaz.\n");
        printf("____________________________________________________________________\n");

        do {
            printf("Unesite svoj izbor: ");
            if (scanf("%d", &izbor) != 1) {
                while (getchar() != '\n');
                printf("Neispravan unos. Molimo unesite broj od 1 do 4.\n");
                validInput = 0;
            }
            else {
                validInput = (izbor >= 1 && izbor <= 4);
                if (!validInput) {
                    printf("Neispravan unos. Molimo unesite broj od 1 do 4.\n");
                }
            }
        } while (!validInput);

        switch (izbor) {
        case 1:
            printf("Unesite ime igraca: ");
            scanf("%s", player->ime);
            player->balance = 500.0;
            player->wins = 0;
            player->losses = 0;
            start(player);
            break;
        case 2:
            pravila();
            break;
        case 3:
            sortAndSearchStatistics();
            break;
        case 4:
            validInput = 0;
            do {
                printf("Jeste li sigurni da zelite izaci iz programa? (D za da / N za ne): ");
                scanf(" %c", &izaci);
                izaci = tolower(izaci);
                validInput = (izaci == 'd' || izaci == 'n');
                if (!validInput) {
                    printf("Neispravan unos. Molimo unesite D za da ili N za ne.\n");
                }
            } while (!validInput);

            if (izaci == 'd') {
                printf("\nVidimo se uskoro!\n");
                exit(0);
            }
            break;
        }
    }
}
