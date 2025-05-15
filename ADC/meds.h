#pragma once

#include <stdio.h>

typedef struct {
	int id;
	char name[20];
	float dose;
	int quantity;
}drug;

void createDrug();
void* listDrugs(drug* const drugMatrix);
void* searchDrug();
void updateDrug();
void deleteDrug();
void saveDrugs();
void loadDrugs();