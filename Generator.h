#pragma once
#include <iostream>
#include <list>
#include <array>
#include "room.h"


using namespace std;

class Generator {
public:
	Generator();
	~Generator();

	static const int NUM_ROOMS = 10;
	void setParams(int x, int y, int variance);
	void generate();

private:
	int x = 7;
	int y = 7;
	int variance = 3;

	int maxEnemies = 2;
	int diffInc = 1;

	Room rooms[NUM_ROOMS];
};