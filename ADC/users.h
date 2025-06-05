#pragma once

#include <stdio.h>

#define MAX_USERS 100
#define MAX_NAME_LEN 20

extern int numOfUsers;

typedef enum{
	NURSE = 1,
	DOCTOR
}role;

typedef struct {
	int id;
	char username[20];
	char password[20];
	role role;
}user;

void addUser();
void deleteUser(user* userMatrix, user* foundUser, int* count);
void listUsers(user* userMatrix, int count);
void saveUsers(user* userMatrix, int count);
user* loadUsers(int* count);
user* login(user* users, int count);