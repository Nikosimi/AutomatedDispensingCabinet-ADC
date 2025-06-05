#pragma once

#include <stdio.h>
#include <stdbool.h>

typedef struct {
	int id;
	char name[20];
	float dose;
	int quantity;
	int addedByUserId;
} drug;

extern int numOfDrugs;

drug* loadDrugs(int* count);
void createDrug();
void listDrugs(drug* const drugMatrix, int count);
drug* searchDrug(drug* const drugMatrix, int count);
void updateDrug(drug* foundDrug);
void deleteDrug(drug* drugMatrix, drug* foundDrug);
void saveDrugs(drug* drugMatrix, int count);
