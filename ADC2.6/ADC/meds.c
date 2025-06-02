#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "meds.h"
static int numOfDrugs = 0;

void loadDrugs() {
	FILE* file = fopen("drugs.bin", "rb");
	if (file == NULL) {
		perror("Ucitavanje lijekova iz datoteke drugs.bin");
		exit(EXIT_FAILURE);
	}
	fread(&numOfDrugs, sizeof(int), 1, file);
	printf("Broj lijekova: %d\n", numOfDrugs);
	drug* drugMatrix = (drug*)calloc(numOfDrugs, sizeof(drug));
	if (drugMatrix == NULL) {
		perror("Zauzimanje memorije za lijekove");
		exit(EXIT_FAILURE);
	}
	fread(drugMatrix, sizeof(drug), numOfDrugs, file);
	return drugMatrix;
}

void createDrug(){
	FILE* file = fopen("drugs.bin", "wb");
	if (file == NULL) {
		perror("Dodavanje lijeka u datoteku drugs.bin");
		exit(EXIT_FAILURE);
	}
	fread(&numOfDrugs, sizeof(int), 1, file);
	printf("Broj lijekova: %d\n", numOfDrugs);
	drug temp = { 0 };
	temp.id = numOfDrugs;
	getchar();
	printf("Unesite ime lijeka.\n");
	scanf("%19[^\n]", temp.name);
	printf("Unesite dozu lijeka.\n");
	scanf("%d", temp.dose);
	printf("Unesite kolicinu lijekova\n");
	scanf("%d", temp.quantity);
	fseek(file, sizeof(drug) * numOfDrugs, SEEK_CUR);
	fwrite(&temp, sizeof(drug), 1, file);
	rewind(file);
	numOfDrugs++;
	fwrite(&numOfDrugs, sizeof(int), 1, file);
	fclose(file);
}

void* listDrugs(drug* const drugMatrix){
if (drugMatrix == NULL) {
		printf("Polje lijekova je prazno.\n");
		return;
	}
	int i;
	for (i = 0; i < numOfDrugs; i++) {
		printf("\tID: %d\tIme lijeka: %s\tDoza: %f\tKolicina: %d", (drugMatrix + i)->id, (drugMatrix + i)->name, (drugMatrix + i)->dose, (drugMatrix + i)->quantity);
	}

}

void* searchDrug(drug* const drugMatrix) {
	if (drugMatrix == NULL) {
		printf("Polje lijekova je prazno.\n");
		return NULL;
	}
	int i;
	char filterDrug[20] = { '\0' };
	printf("Unesite ID ili ime lijeka za pronalazak.\n");
	getchar();
	scanf("%19[\n]", filterDrug);
	for (i = 0; i < numOfDrugs; i++) {
		if (!strcmp(filterDrug, (drugMatrix + i)->name) || !strcmp(filterDrug, (drugMatrix + i)->id)) {
			printf("Lijek je pronaden.\n");
			return (drugMatrix + i);
		}
	}
	return NULL;
}

