#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "meds.h"
#include "users.h"

int numOfDrugs = 0;
int lastUsedId = 0;
int sortName(const void* a, const void* b) {
	const drug* d1 = (const drug*)a;
	const drug* d2 = (const drug*)b;
	return strcmp(d1->name, d2->name);
}
int sortId(const void* a, const void* b) {
	const drug* d1 = (const drug*)a;
	const drug* d2 = (const drug*)b;
	return d1->id - d2->id;
}
int sortQuant(const void* a, const void* b) {
	const drug* d1 = (const drug*)a;
	const drug* d2 = (const drug*)b;
	return d1->quantity - d2->quantity;
}

drug* searchId(drug* array, int left, int right, int id) {
	if (left > right) {
		return NULL;
	}
	int mid = left + (right - left) / 2;
	if (array[mid].id == id) {
		return &array[mid];
	}
	if (array[mid].id > id) {
		return searchId(array, left, mid - 1, id);
	}
	else {
		return searchId(array, mid + 1, right, id);
	}
}

drug* searchName(drug* array, int n, const char* name) {
	if (n <= 0) {
		return NULL;
	}
	if (strcmp(array[0].name, name) == 0) {
		return &array[0];
	}
	return searchName(array + 1, n - 1, name);
}

drug* loadDrugs(int* count) {
	FILE* file = fopen("drugs.bin", "rb");
	if (file == NULL) {
		perror("Ucitavanje lijekova iz datoteke drugs.bin");
		return NULL;
	}
	fread(&numOfDrugs, sizeof(int), 1, file);
	printf("Broj lijekova: %d\n", numOfDrugs);
	drug* drugMatrix = (drug*)calloc(numOfDrugs, sizeof(drug));
	if (drugMatrix == NULL) {
		perror("Zauzimanje memorije za lijekove");
		fclose(file);
		return NULL;
	}
	fread(drugMatrix, sizeof(drug), numOfDrugs, file);
	fclose(file);
	*count = numOfDrugs;
	lastUsedId = 0;
	for (int i = 0; i < numOfDrugs; i++) {
		if (drugMatrix[i].id > lastUsedId)
			lastUsedId = drugMatrix[i].id;
	}
	return drugMatrix;
}

void createDrug(user* loggedInUser) {
	FILE* file = fopen("drugs.bin", "r+b");
	if (file == NULL) {
		file = fopen("drugs.bin", "w+b");
		if (file == NULL) {
			perror("Kreiranje datoteke drugs.bin");
			exit(EXIT_FAILURE);
		}
		numOfDrugs = 0;
		fwrite(&numOfDrugs, sizeof(int), 1, file);
		rewind(file);
	}
	fread(&numOfDrugs, sizeof(int), 1, file);
	drug temp = { 0 };
	temp.id = lastUsedId + 1;
	lastUsedId++;
	printf("Unesite ime lijeka.\n");
	scanf("%19s", temp.name);
	printf("Unesite dozu lijeka.\n");
	scanf("%f", &temp.dose);
	printf("Unesite kolicinu lijekova\n");
	scanf("%d", &temp.quantity);
	temp.createdByUserId = loggedInUser->id;
	fseek(file, sizeof(int) + sizeof(drug) * numOfDrugs, SEEK_SET);
	fwrite(&temp, sizeof(drug), 1, file);
	rewind(file);
	numOfDrugs++;
	fwrite(&numOfDrugs, sizeof(int), 1, file);
	fclose(file);
}


void listDrugs(drug* const drugMatrix, int count, user* loggedInUser) {
	if (drugMatrix == NULL) {
		printf("Polje lijekova je prazno.\n");
		return;
	}
	printf("Sortirati po: 1. Imenu, 2. ID-u, 3. Kolicini\n");
	int sortChoice;
	scanf("%d", &sortChoice);
	switch (sortChoice) {
	case 1:
		qsort(drugMatrix, count, sizeof(drug), sortName);
		break;
	case 2:
		qsort(drugMatrix, count, sizeof(drug), sortId);
		break;
	case 3:
		qsort(drugMatrix, count, sizeof(drug), sortQuant);
		break;
	default:
		printf("Nevazeci odabir.");
		break;
	}
	int i;
	for (i = 0; i < count; i++) {
		printf("\tID: %d\tIme lijeka: %s\tDoza: %.2fmg\tKolicina: %d\tDodao korisnik: %s\n", (drugMatrix + i)->id, (drugMatrix + i)->name, (drugMatrix + i)->dose, (drugMatrix + i)->quantity, loggedInUser->username);
	}
}

drug* searchDrug(drug* const drugMatrix, int count) {
	if (drugMatrix == NULL || count == 0) {
		printf("Polje lijekova je prazno. \n");
		return NULL;
	}
	int searchType;
	printf("Pretraga po:\n1. Po imenu\n2. Po ID-u\nOdabir: ");
	do {
		scanf("%d", &searchType);
		getchar();
		if (searchType == 1) {
			char filterDrug[20] = {'\0'};
			printf("Unesite ime lijeka za pronalazak.\n");
			scanf("%19s", filterDrug);
			drug* found = searchName(drugMatrix, count, filterDrug);
			if (found) {
				printf("Lijek je pronaden.\n");
				printf("\tID: %d\tIme lijeka: %s\tDoza: %.2fmg\tKolicina: %d\n", found->id, found->name, found->dose, found->quantity);
				return found;
			}
		}
		else if (searchType == 2) {
			int filterID;
			printf("Unesite ID lijeka.\n");
			scanf("%d", &filterID);
			qsort(drugMatrix, count, sizeof(drug), sortId);
			drug* found = searchId(drugMatrix, 0, count - 1, filterID);
			if (found) {
				printf("Lijek je pronaden.\n");
				printf("\tID: %d\tIme lijeka: %s\tDoza: %.2fmg\tKolicina: %d\n", found->id, found->name, found->dose, found->quantity);
				return found;
			}
		}
		else {
			printf("Nevazeci odabir. \n");
		}
	} while (searchType != 1 && searchType != 2);
	printf("Lijek nije pronaden. \n");
	return NULL;
}

void updateDrug(drug* foundDrug) {
	printf("Unesite novo ime lijeka: ");
scanf("%19s", foundDrug->name);
	printf("Unesite novu dozu (mg): ");
	scanf("%f", &foundDrug->dose);
	printf("Unesite novu kolicinu: ");
	scanf("%d", &foundDrug->quantity);
	printf("Lijek je azuriran.\n");
}

void deleteDrug(drug* drugMatrix, drug* foundDrug) {
	FILE* file = fopen("drugs.bin", "r+b");
	if (!file) {
		perror("Otvaranje datoteke");
		return;
	}int index = -1;
	for (int i = 0; i < numOfDrugs; i++) {
		if (&drugMatrix[i] == foundDrug) {
			index = i;
			break;
		}
	}
	if (index == -1) {
		printf("Lijek nije pronaden u polju.\n");
		fclose(file);
		return;
	}
	for (int i = index; i < numOfDrugs - 1; i++) {
		drugMatrix[i] = drugMatrix[i + 1];
	}
	numOfDrugs--;
	rewind(file);
	fwrite(&numOfDrugs, sizeof(int), 1, file);
	fseek(file, sizeof(int), SEEK_SET);
	fwrite(drugMatrix, sizeof(drug), numOfDrugs, file);
	fclose(file);

	printf("Lijek je uspjesno izbrisan.\n");
}

void saveDrugs(drug* drugMatrix, int count) {
	FILE* file = fopen("drugs.bin", "wb");
	if (file == NULL) {
		perror("Spremanje lijekova u datoteku drugs.bin");
		return;
	}
	fwrite(&count, sizeof(int), 1, file);
	fwrite(drugMatrix, sizeof(drug), count, file);
	fclose(file);
}