#include "zaglavlje.h"

int vrijednostRuke(int ruka[], int brojKarata) {
    int bodovi = 0;
    int brojAsova = 0;
    for (int i = 0; i < brojKarata; i++) {
        if (ruka[i] == 11) {
            brojAsova++;
        }
        bodovi += ruka[i];
    }

    while (bodovi > 21 && brojAsova > 0) {
        bodovi -= 10;
        brojAsova--;
    }
    return bodovi;
}

void ispisiRuku(int ruka[], int brojKarata) {
    for (int i = 0; i < brojKarata; i++) {
        printf("%d ", ruka[i]);
    }
    printf("\n");
}

void pravila() {
    system("cls");

    FILE* fp = fopen("pravila.txt", "r");
    if (fp == NULL) {
        handleError("Ne mogu otvoriti datoteku");
        return;
    }

    char c;
    while ((c = fgetc(fp)) != EOF) {
        printf("%c", c);
    }
    if (ferror(fp)) {
        handleError("Greska pri citanju datoteke");
    }
    getchar();
    fclose(fp);
}

int start(Player* player) {
    system("cls");
    srand((unsigned)time(NULL));
    int nastaviti = 1;
    int validInput = 0;

    while (nastaviti) {
        int spil[52] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10,
                         2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10,
                         2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10,
                         2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10,
                         11, 11, 11, 11 };

        HAND* rukaDiler = izradiHand();
        HAND* rukaIgrac = izradiHand();
        int bodoviDiler, bodoviIgrac;
        char izb;
        float ulog = 0;

        printf("Diler: Dobrodosli za stol %s! :)\n\n", player->ime);
        printf("Vas novcanik: %.2f $\n", player->balance);

        if (player->balance == 0.0) {
            bankrot(player);
        }

        do {
            printf("Dodajte ulog: ");
            if (scanf("%f", &ulog) != 1) {
                while (getchar() != '\n');
                printf("Neispravan unos. Molimo unesite iznos u brojcanom formatu.\n");
            }
            else {
                validInput = (ulog > 0 && ulog <= player->balance);
                if (!validInput) {
                    if (ulog <= 0) {
                        printf("Ulog ne smije biti nula!\n");
                    }
                    else {
                        printf("Nemate dovoljno sredstava na racunu.\n");
                    }
                }
            }
        } while (!validInput);

        if (ulog <= player->balance) {
            dodajKartu(rukaDiler, spil[rand() % 52]);
            dodajKartu(rukaDiler, spil[rand() % 52]);
            dodajKartu(rukaIgrac, spil[rand() % 52]);
            dodajKartu(rukaIgrac, spil[rand() % 52]);

            printf("\nTvoja ruka:\n");
            ispisiRuku(rukaIgrac->karte, rukaIgrac->velicina);
            bodoviIgrac = vrijednostRuke(rukaIgrac->karte, rukaIgrac->velicina);
            printf("Vrijednost tvoje ruke: %d\n\n", bodoviIgrac);

            printf("Ruka dilera:\n");
            ispisiRuku(rukaDiler->karte, rukaDiler->velicina);
            bodoviDiler = vrijednostRuke(rukaDiler->karte, rukaDiler->velicina);
            printf("Vrijednost dilerove ruke: %d\n\n", bodoviDiler);

            if (bodoviIgrac == 21) {
                player->balance += (ulog * 1.5);
                player->wins++;
                printf("\n\nImate Blackjack!!! Pobijedili ste! (+%.2f$)\n\n", ulog * 1.5);
                zavrsiIgru(player, rukaDiler, rukaIgrac, ulog, bodoviIgrac, bodoviDiler);
                continue;
            }

            if (bodoviDiler == 21) {
                player->balance -= ulog;
                player->losses++;
                printf("\n\nDiler ima blackjack. Vise srece drugi put! :( (-%.2f$)\n\n", ulog);
                zavrsiIgru(player, rukaDiler, rukaIgrac, ulog, bodoviIgrac, bodoviDiler);
                continue;
            }

            int busted = 0;
            while (1) {
                printf("Zelite li vuci novu kartu? (D/N): ");
                scanf(" %c", &izb);

                if (izb == 'n' || izb == 'N') {
                    break;
                }
                if (izb == 'D' || izb == 'd') {
                    dodajKartu(rukaIgrac, spil[rand() % 52]);
                    printf("\n\nTvoje nove karte su:\n");
                    ispisiRuku(rukaIgrac->karte, rukaIgrac->velicina);
                    bodoviIgrac = vrijednostRuke(rukaIgrac->karte, rukaIgrac->velicina);
                    printf("Vrijednost tvoje ruke: %d\n", bodoviIgrac);
                    printf("Vrijednost dilerove ruke: %d\n\n", bodoviDiler);

                    if (bodoviIgrac > 21) {
                        player->balance -= ulog;
                        player->losses++;
                        printf("\nPremasili ste 21! Izgubili ste. :( (-%.2f$)\n\n", ulog);
                        busted = 1;
                        break;
                    }
                }
                else {
                    printf("Nepoznat unos. (D/N)\n");
                }
            }

            if (busted) {
                zavrsiIgru(player, rukaDiler, rukaIgrac, ulog, bodoviIgrac, bodoviDiler);
                continue;
            }

            while (bodoviDiler < 17) {
                dodajKartu(rukaDiler, spil[rand() % 52]);
                bodoviDiler = vrijednostRuke(rukaDiler->karte, rukaDiler->velicina);
                printf("\nDiler uzima kartu.\n");
            }

            printf("\nKarte dilera su:\n");
            ispisiRuku(rukaDiler->karte, rukaDiler->velicina);
            printf("Vrijednost dilerove ruke: %d\n", bodoviDiler);
            printf("Vrijednost tvoje ruke: %d\n", bodoviIgrac);

            if (bodoviDiler == bodoviIgrac) {
                printf("\n\nNerijeseno. (+0$)\n");
            }
            else if (bodoviDiler > 21 || (bodoviIgrac <= 21 && bodoviIgrac > bodoviDiler)) {
                player->balance += ulog;
                player->wins++;
                printf("\n\nPobijedili ste! (+%.2f$)\n\n", ulog);
            }
            else {
                player->balance -= ulog;
                player->losses++;
                printf("\n\nDiler pobjeduje. Vise srece drugi put! :( (-%.2f$)\n\n", ulog);
            }

            zavrsiIgru(player, rukaDiler, rukaIgrac, ulog, bodoviIgrac, bodoviDiler);
        }
    }

    return 0;
}

void zavrsiIgru(Player* player, HAND* rukaDiler, HAND* rukaIgrac, float ulog, int bodoviIgrac, int bodoviDiler) {
    freeHand(rukaDiler);
    freeHand(rukaIgrac);
    int nastaviti;
    char ispisati_statistiku;
    int validInput = 0;

    do {
        printf("Zelite li igrati ponovo? (1 za da / 0 za ne): ");
        if (scanf("%d", &nastaviti) != 1) {
            while (getchar() != '\n');
            printf("Neispravan unos. Molimo unesite 1 za da ili 0 za ne.\n");
        }
        else {
            validInput = (nastaviti == 1 || nastaviti == 0);
            if (!validInput) {
                printf("Neispravan unos. Molimo unesite 1 za da ili 0 za ne.\n");
            }
        }
    } while (!validInput);

    if (nastaviti == 0) {
        validInput = 0;

        do {
            printf("Zelite li ispisati statistiku? (D za da / N za ne): ");
            scanf(" %c", &ispisati_statistiku);
            ispisati_statistiku = tolower(ispisati_statistiku);
            validInput = (ispisati_statistiku == 'd' || ispisati_statistiku == 'n');
            if (!validInput) {
                printf("Neispravan unos. Molimo unesite D za da ili N za ne.\n");
            }
        } while (!validInput);

        if (ispisati_statistiku == 'd') {
            ispisiStatistiku(player);
        }
        mainMenu(player);
    }
    else if (nastaviti == 1) {
        system("cls");
    }
    else {
        printf("Nepoznat unos. Povratak na glavni izbornik.\n");
        mainMenu(player);
    }
}

HAND* izradiHand() {
    HAND* hand = (HAND*)malloc(sizeof(HAND));
    hand->karte = (int*)malloc(10 * sizeof(int));
    hand->velicina = 0;
    hand->kapacitet = 10;
    return hand;
}

void dodajKartu(HAND* hand, int card) {
    if (hand->velicina >= hand->kapacitet) {
        hand->kapacitet *= 2;
        hand->karte = (int*)realloc(hand->karte, hand->kapacitet * sizeof(int));
    }
    hand->karte[hand->velicina++] = card;
}

void freeHand(HAND* hand) {
    if (hand) {
        if (hand->karte) {
            free(hand->karte);
            hand->karte = NULL;
        }
        free(hand);
        hand = NULL;
    }
}

void bankrot(Player* player) {
    printf("\nNa zalost, %s, bankrotirali ste!\n", player->ime);
    printf("Vas balans je: %.2f$\n", player->balance);
    mainMenu(player);
}
