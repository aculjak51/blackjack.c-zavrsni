#include "zaglavlje.h"

int ukupnoDilaneKarte = 0;

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
    if (brojKarata == 0) {
        printf("\n");
        return;
    }
    printf("%d ", ruka[0]);
    ispisiRuku(&ruka[1], brojKarata - 1);  //rekurzija :OOOO
}


void pravila() {
    system("cls");

    FILE* fp = fopen("pravila.txt", "r");
    if (fp == NULL) {
        errPoruka("Ne mogu otvoriti datoteku");
        return;
    }

    char c;
    while ((c = fgetc(fp)) != EOF) {
        printf("%c", c);
    }
    if (ferror(fp)) {
        errPoruka("Greska pri citanju datoteke");
    }
    getchar();
    fclose(fp);
}

int start(Player* player) {
    srand((unsigned)time(NULL));
    int nastaviti = 1;
    int validInput = 0;

    while (nastaviti) {
        int spil[DECKSIZE] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10,
                         2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10,
                         2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10,
                         2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10,
                         11, 11, 11, 11 };

        HAND* rukaDiler = izradiHand();
        HAND* rukaIgrac = izradiHand();
        int bodoviDiler, bodoviIgrac;
        char izb;
        float ulog = 0;

        system("cls");

        printf("Diler: Dobrodosli za stol %s! :)\n\n", player->ime);
        printf("Vas novcanik: %.2f $\n", player->balance);

        if (player->balance == 0.0) {
            bankrot(player);
        }

        do {
            printf("Dodajte ulog: ");
            if (scanf("%f", &ulog) != 1) {
                cistiBuffer();
                printf("Neispravan unos. Molimo unesite iznos u brojcanom formatu.\n");
                validInput = 0;
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

        cistiBuffer();

        dodajKartu(rukaDiler, spil[rand() % 52]);
        dodajKartu(rukaDiler, spil[rand() % 52]);
        dodajKartu(rukaIgrac, spil[rand() % 52]);
        dodajKartu(rukaIgrac, spil[rand() % 52]);

        printf("\nTvoja ruka:\n");
        ispisiRuku(rukaIgrac->karte, rukaIgrac->brojKarata);
        bodoviIgrac = vrijednostRuke(rukaIgrac->karte, rukaIgrac->brojKarata);
        printf("Vrijednost tvoje ruke: %d\n\n", bodoviIgrac);

        printf("Ruka dilera:\n");
        ispisiRuku(rukaDiler->karte, rukaDiler->brojKarata);
        bodoviDiler = vrijednostRuke(rukaDiler->karte, rukaDiler->brojKarata);
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
            validInput = 0;
            do {
                printf("Zelite li vuci novu kartu? (D/N): ");
                scanf(" %c", &izb);
                while (getchar() != '\n');
                izb = tolower(izb);
                validInput = (izb == 'd' || izb == 'n');
                if (!validInput) {
                    printf("Neispravan unos. Molimo unesite D za da ili N za ne.\n");
                }
            } while (!validInput);

            if (izb == 'd') {
                dodajKartu(rukaIgrac, spil[rand() % 52]);
                ukupnoDilaneKarte++;
                printf("\n\nTvoje nove karte su:\n");
                ispisiRuku(rukaIgrac->karte, rukaIgrac->brojKarata);
                bodoviIgrac = vrijednostRuke(rukaIgrac->karte, rukaIgrac->brojKarata);
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
            if (izb == 'n') {
                break;
            }
        }

        if (busted) {
            zavrsiIgru(player, rukaDiler, rukaIgrac, ulog, bodoviIgrac, bodoviDiler);
            continue;
        }

        while (bodoviDiler < 17) {
            dodajKartu(rukaDiler, spil[rand() % 52]);
            ukupnoDilaneKarte++;
            bodoviDiler = vrijednostRuke(rukaDiler->karte, rukaDiler->brojKarata);
            printf("\nDiler uzima kartu.\n");
        }

        printf("\nKarte dilera su:\n");
        ispisiRuku(rukaDiler->karte, rukaDiler->brojKarata);
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

    return 0;
}


void zavrsiIgru(Player* player, HAND* rukaDiler, HAND* rukaIgrac, float ulog, int bodoviIgrac, int bodoviDiler) {
    static int ukupnoIgrica = 0;  // static koristen
    ukupnoIgrica++;

    freeHand(rukaDiler);
    freeHand(rukaIgrac);
    char nastaviti;
    char ispisatiStatistiku;
    int validInput = 0;

    do {
        printf("Zelite li igrati ponovo? (D za da / N za ne): ");
        scanf(" %c", &nastaviti);
        nastaviti = tolower(nastaviti);
        validInput = (nastaviti == 'd' || nastaviti == 'n');
        if (!validInput) {
            printf("Neispravan unos. Molimo unesite D za da ili N za ne.\n");
            cistiBuffer();
        }
        else {
            cistiBuffer();
        }
    } while (!validInput);

    if (nastaviti == 'n') {
        validInput = 0;

        do {
            printf("Zelite li ispisati statistiku? (D za da / N za ne): ");
            scanf(" %c", &ispisatiStatistiku);
            ispisatiStatistiku = tolower(ispisatiStatistiku);
            validInput = (ispisatiStatistiku == 'd' || ispisatiStatistiku == 'n');
            if (!validInput) {
                printf("Neispravan unos. Molimo unesite D za da ili N za ne.\n");
                cistiBuffer();
            }
            else {
                cistiBuffer();
            }
        } while (!validInput);

        if (ispisatiStatistiku == 'd') {
            ispisiStatistiku(player);
        }

        printf("Ukupno odigranih igara: %d\n", ukupnoIgrica);
        printf("Ukupno podjeljenih karata: %d\n\n\n", ukupnoDilaneKarte);
        mainMenu(player);
    }
}

HAND* izradiHand() {
    HAND* hand = (HAND*)malloc(sizeof(HAND));
    hand->karte = (int*)malloc(10 * sizeof(int));
    hand->brojKarata = 0;
    hand->kapacitet = 10;
    return hand;
}

void dodajKartu(HAND* hand, int card) {
    if (hand->brojKarata >= hand->kapacitet) {
        hand->kapacitet *= 2;
        hand->karte = (int*)realloc(hand->karte, hand->kapacitet * sizeof(int));
    }
    hand->karte[hand->brojKarata++] = card;
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
