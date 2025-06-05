#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "users.h"

int numOfUsers = 0;

void addUser(){
    FILE* file = fopen("users.bin", "r+b");
    if (file == NULL) {
        file = fopen("users.bin", "w+b");
        if (file == NULL) {
            perror("Kreiranje datoteke users.bin");
            exit(EXIT_FAILURE);
        }
        numOfUsers = 0;
        fwrite(&numOfUsers, sizeof(int), 1, file);
        rewind(file);
    }
    fread(&numOfUsers, sizeof(int), 1, file);
    user temp = { 0 };
    temp.id = numOfUsers;
    getchar();
    printf("Unesite korisničko ime:\n");
    fgets(temp.username, sizeof(temp.username), stdin);
    temp.username[strcspn(temp.username, "\n")] = 0;
    printf("Unesite lozinku:\n");
    fgets(temp.password, sizeof(temp.password), stdin);
    temp.password[strcspn(temp.password, "\n")] = 0;
    int roleInput;
    do {
        printf("Unesite ulogu (1 = NURSE, 2 = DOCTOR):\n");
        scanf("%d", &roleInput);
    } while (roleInput != NURSE && roleInput != DOCTOR);
    temp.role = (role)roleInput;
    fseek(file, sizeof(int) + sizeof(user) * numOfUsers, SEEK_SET);
    fwrite(&temp, sizeof(user), 1, file);
    numOfUsers++;
    rewind(file);
    fwrite(&numOfUsers, sizeof(int), 1, file);
    fclose(file);
    printf("Korisnik uspjesno dodan!\n");
}
void deleteUser(user* userMatrix, user* foundUser, int* count) {
    FILE* file = fopen("users.bin", "r+b");
    if (!file) {
        perror("Otvaranje datoteke");
        return;
    }

    int index = -1;
    for (int i = 0; i < *count; i++) {
        if (&userMatrix[i] == foundUser) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Korisnik nije pronađen.\n");
        fclose(file);
        return;
    }

    for (int i = index; i < *count - 1; i++) {
        userMatrix[i] = userMatrix[i + 1];
    }

    (*count)--;

    rewind(file);
    fwrite(count, sizeof(int), 1, file);
    fseek(file, sizeof(int), SEEK_SET);
    fwrite(userMatrix, sizeof(user), *count, file);
    fclose(file);

    printf("Korisnik je uspjesno izbrisan.\n");
}
void listUsers(user* userMatrix, int count) {
    if (userMatrix == NULL || count == 0) {
        printf("Nema korisnika.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        printf("ID: %d\tUsername: %s\tRole: %s\n",
            userMatrix[i].id,
            userMatrix[i].username,
            userMatrix[i].role == NURSE ? "NURSE" : "DOCTOR");
    }
}
void saveUsers(user* userMatrix, int count) {
    FILE* file = fopen("users.bin", "wb");
    if (!file) {
        perror("Spremanje korisnika");
        return;
    }

    fwrite(&count, sizeof(int), 1, file);
    fwrite(userMatrix, sizeof(user), count, file);
    fclose(file);
}
user* loadUsers(int* count) {
    FILE* file = fopen("users.bin", "rb");
    if (!file) {
        *count = 0;
        return NULL;
    }

    fread(count, sizeof(int), 1, file);
    user* userMatrix = (user*)calloc(*count, sizeof(user));
    if (!userMatrix) {
        perror("Alokacija korisnika");
        exit(EXIT_FAILURE);
    }

    fread(userMatrix, sizeof(user), *count, file);
    fclose(file);
    return userMatrix;
}
user* login(user* users, int count) {
    char username[20], password[20];
    printf("Korisnicko ime: ");
    scanf("%19s", username);
    printf("Lozinka: ");
    scanf("%19s", password);

    for (int i = 0; i < count; i++) {
        if (strcmp(users[i].username, username) == 0 &&
            strcmp(users[i].password, password) == 0) {
            printf("Prijava uspjesna! Dobrodosli, %s\n", users[i].username);
            return &users[i];
        }
    }

    printf("Neispravni podaci. Prijava nije uspjela.\n");
    return NULL;
}
