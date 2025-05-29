#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "meds.h"

int numOfDrugs = 0;

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
	temp.id = numOfDrugs;
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
	int i;
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
	printf("Lijek nije pronaden.\n");
	return NULL;
}

void updateDrug(const char* oldName){

}