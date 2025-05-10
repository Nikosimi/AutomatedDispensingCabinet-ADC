#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "meds.h"
#include "users.h"
#include "utils.h"

typedef enum {
	EXIT,
	ADD_DRUG,
	LIST_DRUGS,
	UPDATE_DRUG,
	DELETE_DRUG,
	ADD_USER,
	DELETE_USER,
	LIST_USERS
}MENU;

void printMenu() {
	printf("\n=== ADC Sustav ===\n");
	printf("1. Dodaj lijek\n");
	printf("2. Prikazi lijekove\n");
	printf("3. Azuriraj lijek\n");
	printf("4. Dodaj korisnika\n");
	printf("5. Izbrisi korisnika\n");
	printf("6. Prikazi korisnike\n");
	printf("0. Izlaz\n");
	printf("Odabir: ");
}

int main(void) {
	int choice;
	loadDrugs();
	loadUsers();

	do {
		printMenu();
		scanf("%d", &choice);
		clearInputBuffer();

		switch (choice)
		{
		case ADD_DRUG: createDrug(); 
			break;
		case LIST_DRUGS: listDrugs(); 
			break;
		case UPDATE_DRUG: updateDrug(); 
			break;
		case DELETE_DRUG: deleteDrug(); 
			break;
		case ADD_USER: addUser(); 
			break;
		case DELETE_USER: deleteUser(); 
			break;
		case LIST_USERS: listUsers; 
			break;
		case EXIT:
			saveDrugs();
			saveUsers();
			printf("Izlaz iz sustava...\n");
			break;
		default: printf("Nevazeci odabir.\n");
			break;
		}
		if (choice != EXIT) {
			pause();
		}
	} while (choice != EXIT);

	return 0;
}