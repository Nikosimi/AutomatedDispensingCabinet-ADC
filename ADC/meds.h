#pragma once

#include <stdio.h>

typedef struct {
	int id;
	char name[20];
	float dose;
	int quantity;
}drug;

void createDrug();
void deleteDrug();
void listDrugs();
void updateDrug();
void saveDrugs();
void loadDrugs();