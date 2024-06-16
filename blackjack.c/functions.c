#include "zaglavlje.h"

void ispisiStatistiku(Player* player) {
    FILE* file = fopen("statistika.bin", "rb+");
    if (file == NULL) {
        file = fopen("statistika.bin", "wb");
        if (file == NULL) {
            errPoruka("Greska pri otvaranju datoteke");
            return;
        }
    }

    int brojIgraca = 0;
    fread(&brojIgraca, sizeof(int), 1, file);
    brojIgraca++;

    fseek(file, 0, SEEK_SET);
    fwrite(&brojIgraca, sizeof(int), 1, file);
    fseek(file, 0, SEEK_END);
    fwrite(player, sizeof(Player), 1, file);

    fclose(file);
    printf("Statistika upisana u datoteku.\n");
}


void errPoruka(const char* message) {
    fprintf(stderr, "%s: %s\n", message, strerror(errno));
}

void trimWhitespace(char* str) {
    char* end;

    while (isspace((unsigned char)*str)) str++;

    if (*str == 0) {
        return;
    }

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    end[1] = '\0';
}

int strcasecmp(const char* s1, const char* s2) {
    while (*s1 && (tolower(*s1) == tolower(*s2))) {
        s1++;
        s2++;
    }
    return tolower(*(unsigned char*)s1) - tolower(*(unsigned char*)s2);
}

void sortAndSearchStatistics() {
    FILE* file = fopen("statistika.bin", "rb");
    if (file == NULL) {
        errPoruka("Ne mogu otvoriti fajl za citanje statistike");
        return;
    }

    int brojIgraca = 0;
    fread(&brojIgraca, sizeof(int), 1, file);
    if (brojIgraca == 0) {
        printf("Nema zapisa za prikazivanje.\n");
        fclose(file);
        return;
    }

    Player* players = (Player*)malloc(brojIgraca * sizeof(Player));
    if (players == NULL) {
        errPoruka("Greska pri alociranju memorije");
        fclose(file);
        return;
    }

    fread(players, sizeof(Player), brojIgraca, file);
    fclose(file);

    qsort(players, brojIgraca, sizeof(Player), comparePlayers);

    printf("Sortirana statistika igraca:\n");
    for (int i = 0; i < brojIgraca; i++) {
        printf("Ime: %s, Balance: %.2f, Pobijeda: %d, Porazi: %d\n",
            players[i].ime, players[i].balance,
            players[i].wins, players[i].losses);
    }

    char searchName[MAXIME];
    printf("Unesite ime igraca za pretragu: ");
    scanf("%s", searchName);
    trimWhitespace(searchName);

    int found = 0;
    for (int i = 0; i < brojIgraca; i++) {
        if (strcasecmp(players[i].ime, searchName) == 0) {
            printf("Pronadjen igrac:\n");
            printf("Ime: %s, Balance: %.2f, Pobijeda: %d, Porazi: %d\n",
                players[i].ime, players[i].balance,
                players[i].wins, players[i].losses);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Igrac sa imenom %s nije pronadjen.\n", searchName);
    }

    free(players);

}

int comparePlayers(const void* a, const void* b) {
    Player* playerA = (Player*)a;
    Player* playerB = (Player*)b;
    if (playerB->balance > playerA->balance) return 1;
    if (playerB->balance < playerA->balance) return -1;
    return 0;
}

void removeStats() {
    if (remove("statistika.bin") != 0) {
        errPoruka("Greska pri brisanju datoteke");
    }
    else {
        printf("Datoteka statistika.bin uspjesno obrisana\n");
    }

}

void inline cistiBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void mainMenu(Player* player) {
    MenuOption izbor;
    char izaci;
    char del;
    int validInput;

    while (1) {
        printf("\t\tDobrodosli u BLACKJACK!!!\n");
        printf("____________________________________________________________________\n");
        printf("1. Zaigraj!\n");
        printf("2. Procitaj pravila igre.\n");
        printf("3. Pretrazi statistiku.\n");
        printf("4. Obrisi stats file.\n");
        printf("5. Izlaz.\n");
        printf("____________________________________________________________________\n");

        do {
            printf("Unesite svoj izbor: ");
            if (scanf("%d", (int*)&izbor) != 1) {
                printf("Neispravan unos. Molimo unesite broj od 1 do 5.\n");
                validInput = 0;
                cistiBuffer();
            }
            else {
                validInput = (izbor >= IGRATI && izbor <= IZLAZ);
                if (!validInput) {
                    printf("Neispravan unos. Molimo unesite broj od 1 do 5.\n");
                    cistiBuffer();
                }
            }
        } while (!validInput);

        switch (izbor) {
        case IGRATI:
            cistiBuffer();
            printf("Unesite ime igraca: ");
            scanf("%49s", player->ime);
            player->balance = 500.0;
            player->wins = 0;
            player->losses = 0;
            start(player);
            break;
        case PRAVILA:
            pravila();
            break;
        case STATISTIKA:
            sortAndSearchStatistics();
            break;
        case OBRISI_STATS:
            do {
               cistiBuffer();
                printf("Jeste li sigurni da zelite izbrisati stats file? (D za da / N za ne): ");
                scanf(" %c", &del);
                del = tolower(del);
                validInput = (del == 'd' || del == 'n');
                if (!validInput) {
                    printf("Neispravan unos. Molimo unesite D za da ili N za ne.\n");
                    cistiBuffer();
                }
            } while (!validInput);

            if (del == 'd') {
                removeStats();
            }
            break;

        case IZLAZ:
            validInput = 0;
            do {
                printf("Jeste li sigurni da zelite izaci iz programa? (D za da / N za ne): ");
                scanf(" %c", &izaci);
                izaci = tolower(izaci);
                validInput = (izaci == 'd' || izaci == 'n');
                if (!validInput) {
                    printf("Neispravan unos. Molimo unesite D za da ili N za ne.\n");
                    cistiBuffer();
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

