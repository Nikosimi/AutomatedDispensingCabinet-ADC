#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "meds.h"

int numOfDrugs = 0;
int lastUsedId = 0;

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

void createDrug() {
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
	fseek(file, sizeof(int) + sizeof(drug) * numOfDrugs, SEEK_SET);
	fwrite(&temp, sizeof(drug), 1, file);
	rewind(file);
	numOfDrugs++;
	fwrite(&numOfDrugs, sizeof(int), 1, file);
	fclose(file);
}

void listDrugs(drug* const drugMatrix, int count) {
	if (drugMatrix == NULL) {
		printf("Polje lijekova je prazno.\n");
		return;
	}
	int i;
	for (i = 0; i < count; i++) {
		printf("\tID: %d\tIme lijeka: %s\tDoza: %.2fmg\tKolicina: %d\n", (drugMatrix + i)->id, (drugMatrix + i)->name, (drugMatrix + i)->dose, (drugMatrix + i)->quantity);
	}
}

drug* searchDrug(drug* const drugMatrix, int count) {
	if (drugMatrix == NULL) {
		printf("Polje lijekova je prazno.\n");
		return NULL;
	}
	int i, searchType;
	printf("Pretraga po:\n1. Po imenu\n2. Po ID-u\nOdabir: ");
	scanf("%d", &searchType);
	getchar();
	if (searchType == 1) {
		char filterDrug[20] = { '\0' };
		printf("Unesite ime lijeka za pronalazak.\n");
		scanf("%19s", filterDrug);
		for (i = 0; i < count; i++) {
			if (strcmp(filterDrug, (drugMatrix + i)->name) == 0) {
				printf("Lijek je pronaden.\n");
				printf("\tID: %d\tIme lijeka: %s\tDoza: %.2fmg\tKolicina: %d\n", (drugMatrix + i)->id, (drugMatrix + i)->name, (drugMatrix + i)->dose, (drugMatrix + i)->quantity);
				return (drugMatrix + i);
			}
		}
	}
	else if (searchType == 2) {
		int filterID;
		printf("Unesite ID lijeka.\n");
		scanf("%d", &filterID);
		for (i = 0; i < count; i++) {
			if ((drugMatrix + i)->id == filterID) {
				printf("Lijek je pronaden.\n");
				printf("\tID: %d\tIme lijeka: %s\tDoza: %.2fmg\tKolicina: %d\n", (drugMatrix + i)->id, (drugMatrix + i)->name, (drugMatrix + i)->dose, (drugMatrix + i)->quantity);
				return (drugMatrix + i);
			}
		}
	}
	else {
		printf("Nevazeci odabir.\n");
	}
	printf("Lijek nije pronaden.\n");
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
		printf("Lijek nije pronađen u polju.\n");
		fclose(file);
		return;
	}

	// Pomakni elemente ulijevo
	for (int i = index; i < numOfDrugs - 1; i++) {
		drugMatrix[i] = drugMatrix[i + 1];
	}
	numOfDrugs--;

	// Upisi novi broj i ažurirano polje u datoteku
	rewind(file);
	fwrite(&numOfDrugs, sizeof(int), 1, file);
	fseek(file, sizeof(int), SEEK_SET);
	fwrite(drugMatrix, sizeof(drug), numOfDrugs, file);
	fclose(file);

	printf("Lijek je uspješno izbrisan.\n");
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
