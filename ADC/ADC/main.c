#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "meds.h"
#include "users.h"

#define PAUSE system("pause");
#define CLRSCREEN system("cls");

typedef enum {
	EXIT,
	ADD_DRUG,
	LIST_DRUGS,
	SEARCH_DRUG,
	ADD_USER,
	DELETE_USER,
	LIST_USERS
} MENU;

typedef enum {
	EXIT2,
	UPDATE_DRUG,
	DELETE_DRUG
} SUBMENUDRUGS;

void printMenu() {
	printf("\n=== ADC Sustav ===\n");
	printf("1. Dodaj lijek\n");
	printf("2. Prikazi lijekove\n");
	printf("3. Pretrazi lijek\n");
	printf("4. Dodaj korisnika\n");
	printf("5. Izbrisi korisnika\n");
	printf("6. Prikazi korisnike\n");
	printf("0. Izlaz\n");
	printf("Odabir: ");
}

void printSubMenuDrugs() {
	printf("\n=== ADC Sustav - podizbornik (lijek) ===\n");
	printf("1. Azuriraj lijek\n");
	printf("2. Izbrisi lijek\n");
	printf("0. Izlaz\n");
	printf("Odabir: ");
}

int main(void) {
	int choice, choice2, count = 0, userCount = 0;
	char confirm;
	drug* drugMatrix = loadDrugs(&count);
	drug* foundDrug = NULL;
	user* userMatrix = loadUsers(&userCount);
	user* loggedInUser = NULL;

	if (userCount == 0) {
		printf("Sustav nema korisnika. Kreiranje prvog korisnika:\n");
		addUser(&userMatrix, &userCount);
	}

	do {
		loggedInUser = loginUser(userMatrix, userCount);
	} while (loggedInUser == NULL);

	do {
		PAUSE;
		CLRSCREEN;
		printMenu();
		scanf("%d", &choice);

		switch (choice) {
		case ADD_DRUG:
			createDrug(loggedInUser->id);
			if (drugMatrix != NULL) free(drugMatrix);
			drugMatrix = loadDrugs(&count);
			break;

		case LIST_DRUGS:
			listDrugs(drugMatrix, count, userMatrix, userCount);
			break;

		case SEARCH_DRUG:
			foundDrug = searchDrug(drugMatrix, count);
			if (foundDrug != NULL) {
				do {
					printSubMenuDrugs();
					getchar();
					scanf("%d", &choice2);

					switch (choice2) {
					case UPDATE_DRUG:
						printf("Jeste li sigurni? Y/n ");
						do {
							scanf(" %c", &confirm);
						} while (confirm != 'Y' && confirm != 'n');

						if (confirm == 'Y') {
							updateDrug(foundDrug);
							saveDrugs(drugMatrix, count);
						}
						break;

					case DELETE_DRUG:
						printf("Jeste li sigurni? Y/n ");
						do {
							scanf(" %c", &confirm);
						} while (confirm != 'Y' && confirm != 'n');

						if (confirm == 'Y') {
							deleteDrug(drugMatrix, foundDrug);
							free(drugMatrix);
							drugMatrix = loadDrugs(&count);
						}
						break;

					case EXIT2:
						printf("Izlaz iz podizbornika...\n");
						break;

					default:
						printf("Nevazeci odabir.\n");
						break;
					}
				} while (choice2 != EXIT2);
			}
			break;

		case ADD_USER:
			addUser(&userMatrix, &userCount);
			break;

		case DELETE_USER: {
			int idToDelete;
			printf("Unesite ID korisnika za brisanje: ");
			scanf("%d", &idToDelete);
			user* foundUser = NULL;

			for (int i = 0; i < userCount; i++) {
				if (userMatrix[i].id == idToDelete) {
					foundUser = &userMatrix[i];
					break;
				}
			}

			if (foundUser) {
				deleteUser(userMatrix, foundUser, &userCount);
				free(userMatrix);
				userMatrix = loadUsers(&userCount);
			}
			else {
				printf("Korisnik nije pronaden.\n");
			}
			break;
		}

		case LIST_USERS:
			listUsers(userMatrix, userCount);
			break;

		case EXIT:
			printf("Izlaz iz sustava...\n");
			break;

		default:
			printf("Nevazeci odabir.\n");
			break;
		}
	} while (choice != EXIT);

	if (drugMatrix != NULL) free(drugMatrix);
	if (userMatrix != NULL) free(userMatrix);

	return 0;
}
