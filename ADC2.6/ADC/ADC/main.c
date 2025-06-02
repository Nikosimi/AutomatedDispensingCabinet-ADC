#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "meds.h"
#include "users.h"
#include "utils.h"

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
	printf("6. Dodaj korisnika\n");
	printf("7. Izbrisi korisnika\n");
	printf("8. Prikazi korisnike\n");
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
	int choice, choice2, count = 0, lastUsedId=0;
	bool hasFound=false;
	drug* drugMatrix = loadDrugs(&count);
	drug* foundDrug = NULL;
	char confirm;

	do {
		printMenu();
		scanf("%d", &choice);
		getchar();

		switch (choice)
		{
		case ADD_DRUG:
			createDrug();
			if (drugMatrix != NULL) {
				free(drugMatrix);
			}
			drugMatrix = loadDrugs(&count);
			break;
		case LIST_DRUGS:
			listDrugs(drugMatrix, count);
			break;
		case SEARCH_DRUG:
			foundDrug=searchDrug(drugMatrix, count);
			if (foundDrug!=NULL) {
				do {
					printSubMenuDrugs();
					getchar();
					scanf("%d", &choice2);
					switch (choice2)
					{
						case UPDATE_DRUG:
							if (choice2 == 1) {
								printf("Jeste li sigurni? Y/n ");
								do {
									scanf("%c", &confirm);
								} while (confirm != 'Y' && confirm != 'n');
							}
							if (confirm == 'Y') {
								updateDrug(foundDrug);
								saveDrugs(drugMatrix, count);
							}
							break;
						case DELETE_DRUG:
							if (choice2 == 2) {
								printf("Jeste li sigurni? Y/n ");
								do {
									scanf("%c", &confirm);
								} while (confirm != 'Y' && confirm != 'n');
							}
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
			/*
			case ADD_USER: addUser();
				break;
			case DELETE_USER: deleteUser();
				break;
			case LIST_USERS: listUsers;
				break;*/
		case EXIT:
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

	return 0;
}
