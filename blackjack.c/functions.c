#include "zaglavlje.h"

void ispisiStatistiku(Player* player) {
    FILE* file = fopen("statistika.bin", "rb+");
    if (file == NULL) {
        file = fopen("statistika.bin", "wb");
        if (file == NULL) {
            handleError("Greska pri otvaranju datoteke");
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

int kopirajDatoteku(const char* source, const char* dest) {
    FILE* srcFile = fopen(source, "rb");
    if (srcFile == NULL) {
        handleError("Ne mogu otvoriti izvorni fajl za kopiranje");
        return -1;
    }

    FILE* destFile = fopen(dest, "wb");
    if (destFile == NULL) {
        handleError("Ne mogu otvoriti destinacijski fajl za kopiranje");
        fclose(srcFile);
        return -1;
    }

    fseek(srcFile, 0, SEEK_END);
    long fileSize = ftell(srcFile);
    rewind(srcFile);

    char* buffer = (char*)malloc(fileSize * sizeof(char));
    if (buffer == NULL) {
        handleError("Ne mogu alocirati memoriju za kopiranje fajla");
        fclose(srcFile);
        fclose(destFile);
        return -1;
    }

    size_t bytesRead = fread(buffer, sizeof(char), fileSize, srcFile);
    if (bytesRead != fileSize) {
        handleError("Greska pri citanju iz izvornog fajla");
        free(buffer);
        fclose(srcFile);
        fclose(destFile);
        return -1;
    }

    size_t bytesWritten = fwrite(buffer, sizeof(char), fileSize, destFile);
    if (bytesWritten != fileSize) {
        handleError("Greska pri pisanju u destinacijski fajl");
        free(buffer);
        fclose(srcFile);
        fclose(destFile);
        return -1;
    }

    free(buffer);
    fclose(srcFile);
    fclose(destFile);
    return 0;
}

void handleError(const char* message) {
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
        handleError("Ne mogu otvoriti fajl za citanje statistike");
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
        handleError("Greska pri alociranju memorije");
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
