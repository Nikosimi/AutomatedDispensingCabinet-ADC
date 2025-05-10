#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "meds.h"
static int numOfDrugs = 0;

void createDrug(){
	FILE* file = fopen("drugs.bin", "r+b");
	if (file == NULL) {
		perror("Dodavanje lijeka u datoteke drugs.bin");
		exit(EXIT_FAILURE);
	}
	fread(&numOfDrugs, sizeof(int), 1, file);
	printf("ID lijeka: %d\n", numOfDrugs);
	drug temp = { 0 };
	temp.id = numOfDrugs;
	getchar();
	printf("Unesite ime lijeka!\n");
	scanf("%19s[^\n]", temp.name);
	printf("Unesite dozu lijeka!\n");
	scanf("%d", temp.dose);
	printf("Unesite kolicinu lijeka!\n");
	scanf("%d", temp.quantity);
	fseek(file, sizeof(drug) * numOfDrugs, SEEK_CUR);
	fwrite(&temp, sizeof(drug), 1, file);
	rewind(file);
	numOfDrugs++;
	fwrite(&numOfDrugs, sizeof(int), 1, file);
	fclose(file);
}
