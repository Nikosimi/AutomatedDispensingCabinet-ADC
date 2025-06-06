#pragma once

#include <stdio.h>

extern int numOfUsers;

typedef enum {
    NURSE = 1,
    DOCTOR
} role;

typedef struct {
    int id;
    char username[20];
    char password[20];
    role role;
} user;

user* loadUsers(int* userCount);
void saveUsers(user* userMatrix, int userCount);
void addUser(user** userMatrix, int* userCount);
user* loginUser(user* userMatrix, int userCount);
user* findUserById(user* userMatrix, int userCount, int id);
void deleteUser(user* userMatrix, user* foundUser, int* userCount);
void listUsers(user* userMatrix, int userCount);
