#pragma once

#include <stdio.h>

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
void deleteUser();
void listUsers();
void saveUsers();
void loadUsers();