#pragma once
#include "mario.h"
#include "level.h"

void load(mario* player, int* actualLevel, double* levelTime);
void save(mario* player, int* actualLevel, double* levelTime);
void newGame(mario* player);