#pragma once

#include <stdio.h>
#include <stdbool.h>

typedef struct {
	int id;
	char name[20];
	float dose;
	int quantity;
} drug;

extern int numOfDrugs;

drug* loadDrugs(int* count);
void createDrug();
void listDrugs(drug* const drugMatrix, int count);
drug* searchDrug(drug* const drugMatrix, int count);
void updateDrug(const char* oldName);
void deleteDrug();
void saveDrugs();
