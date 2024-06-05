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

    char searchName[50];
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

