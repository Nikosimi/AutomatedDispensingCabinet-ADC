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
	LIST_USERS,
	DELETE_ALL_DRUGS
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
	printf("7. Izbrisi sve lijekove\n");
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
			CLRSCREEN;
			createDrug(loggedInUser);
			if (drugMatrix != NULL) free(drugMatrix);
			drugMatrix = loadDrugs(&count);
			break;

		case LIST_DRUGS:
			CLRSCREEN;
			listDrugs(drugMatrix, count, loggedInUser);
			break;

		case SEARCH_DRUG:
			CLRSCREEN;
			foundDrug = searchDrug(drugMatrix, count);
			if (foundDrug != NULL) {
				do {
					CLRSCREEN;
					printf("\tID: %d\tIme lijeka: %s\tDoza: %.2fmg\tKolicina: %d\n", foundDrug->id, foundDrug->name, foundDrug->dose, foundDrug->quantity);
					printSubMenuDrugs();
					getchar();
					scanf("%d", &choice2);

					switch (choice2) {
					case UPDATE_DRUG:
						printf("Jeste li sigurni? Y/n ");
						do {
							scanf("%c", &confirm);
						} while (confirm != 'Y' && confirm != 'n');

						if (confirm == 'Y') {
							updateDrug(foundDrug);
							saveDrugs(drugMatrix, count);
						}
						break;

					case DELETE_DRUG:
						printf("Jeste li sigurni? Y/n ");
						do {
							scanf("%c", &confirm);
						} while (confirm != 'Y' && confirm != 'n');

						if (confirm == 'Y') {
							deleteDrug(drugMatrix, foundDrug);
							free(drugMatrix);
							drugMatrix = loadDrugs(&count);
							foundDrug = NULL;
							choice2 = EXIT2;

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
			CLRSCREEN;
			addUser(&userMatrix, &userCount);
			break;

		case DELETE_USER: {
			CLRSCREEN;
			int idToDelete;
			printf("Jeste li sigurni? Y/n ");
			do {
				scanf("%c", &confirm);
			} while (confirm != 'Y' && confirm != 'n');

			if (confirm == 'Y') {
				listUsers(userMatrix, userCount);
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
		}

		case LIST_USERS:
			CLRSCREEN;
			listUsers(userMatrix, userCount);
			break;

		case DELETE_ALL_DRUGS:
			CLRSCREEN
			printf("Jeste li sigurni? Y/n ");
			do {
				scanf("%c", &confirm);
			} while (confirm != 'Y' && confirm != 'n');

			if (confirm == 'Y') {
				if (remove("drugs.bin") == 0)
					printf("Svi lijekovi su obrisani.\n");
				else
					perror("Brisanje datoteke");
			}
			choice = EXIT;
			break;
		case EXIT:
			CLRSCREEN;
			printf("Izlaz iz sustava...\n");
			break;

		default:
			printf("Nevazeci odabir.\n");
			break;
		}
	} while (choice != EXIT);

	if (drugMatrix != NULL) {
		free(drugMatrix);
	}
	if (userMatrix != NULL) {
		free(userMatrix);
	}
	return 0;
}
