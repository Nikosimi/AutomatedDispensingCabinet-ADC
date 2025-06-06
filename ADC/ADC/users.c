#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "users.h"

user* loadUsers(int* userCount) {
    FILE* file = fopen("users.bin", "rb");
    if (!file) {
        *userCount = 0;
        return NULL;
    }
    fread(userCount, sizeof(int), 1, file);
    if (*userCount == 0) {
        fclose(file);
        return NULL;
    }
    user* userMatrix = malloc(sizeof(user) * (*userCount));
    if (!userMatrix) {
        perror("Memorija za korisnike");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    fread(userMatrix, sizeof(user), *userCount, file);
    fclose(file);
    return userMatrix;
}

void saveUsers(user* userMatrix, int userCount) {
    FILE* file = fopen("users.bin", "wb");
    if (!file) {
        perror("Spremanje korisnika");
        return;
    }
    fwrite(&userCount, sizeof(int), 1, file);
    fwrite(userMatrix, sizeof(user), userCount, file);
    fclose(file);
}

void addUser(user** userMatrix, int* userCount) {
    user tempUser = { 0 };
    tempUser.id = (*userCount == 0) ? 1 : (*userMatrix)[*userCount - 1].id + 1;

    printf("Unesite korisnicko ime: ");
    scanf("%19s", tempUser.username);

    printf("Unesite lozinku: ");
    scanf("%19s", tempUser.password);

    printf("Odaberite ulogu (1 = NURSE, 2 = DOCTOR): ");
    int r;
    scanf("%d", &r);
    tempUser.role = (r == 1) ? NURSE : DOCTOR;

    user* temp = realloc(*userMatrix, sizeof(user) * (*userCount + 1));
    if (!temp) {
        perror("Memorija za korisnike");
        return;
    }
    *userMatrix = temp;
    (*userMatrix)[*userCount] = tempUser;
    (*userCount)++;

    saveUsers(*userMatrix, *userCount);
    printf("Korisnik uspjesno dodan.\n");
}

user* loginUser(user* userMatrix, int userCount) {
    if (userCount == 0) {
        printf("Nema korisnika u sustavu.\n");
        return NULL;
    }

    char username[20], password[20];
    printf("Prijava korisnika\n");
    printf("Korisnicko ime: ");
    scanf("%19s", username);
    printf("Lozinka: ");
    scanf("%19s", password);

    for (int i = 0; i < userCount; i++) {
        if (strcmp(userMatrix[i].username, username) == 0 && strcmp(userMatrix[i].password, password) == 0) {
            printf("Uspjesna prijava: %s\n", username);
            return &userMatrix[i];
        }
    }

    printf("Pogresno korisničko ime ili lozinka.\n");
    return NULL;
}

user* findUserById(user* userMatrix, int userCount, int id) {
    for (int i = 0; i < userCount; i++) {
        if (userMatrix[i].id == id) {
            return &userMatrix[i];
        }
    }
    return NULL;
}

void deleteUser(user* userMatrix, user* foundUser, int* userCount) {
    int index = -1;
    for (int i = 0; i < *userCount; i++) {
        if (&userMatrix[i] == foundUser) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Greska: korisnik nije pronaden u polju.\n");
        return;
    }

    for (int i = index; i < *userCount - 1; i++) {
        userMatrix[i] = userMatrix[i + 1];
    }

    (*userCount)--;

    FILE* file = fopen("users.bin", "wb");
    if (!file) {
        perror("Brisanje korisnika");
        return;
    }
    fwrite(userCount, sizeof(int), 1, file);
    fwrite(userMatrix, sizeof(user), *userCount, file);
    fclose(file);

    printf("Korisnik je uspjesno izbrisan.\n");
}
void listUsers(user* userMatrix, int userCount) {
    if (userMatrix == NULL || userCount == 0) {
        printf("Nema registriranih korisnika.\n");
        return;
    }

    printf("\n--- Popis korisnika ---\n");
    for (int i = 0; i < userCount; i++) {
        printf("ID: %d\tKorisnicko ime: %s\tUloga: %s\n",
            userMatrix[i].id,
            userMatrix[i].username,
            (userMatrix[i].role == NURSE) ? "NURSE" : "DOCTOR");
    }
}
