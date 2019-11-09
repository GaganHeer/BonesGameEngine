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
	
	Room* generate();
	int getNumRooms();

	int getWidth(int i);
	int getHeight(int i);
	int getEntryLocation(int i);
	int getEntryDoor(int i);
	int getExitLocation(int i);
	int getExitDoor(int i);
	int getIsStart(int i);
	int getIsEnd(int i);
	int getStairX(int i);
	int getStairY(int i);
	int getCorridorLength(int i);

private:
	int x = 7;
	int y = 7;
	int variance = 3;

	int maxEnemies = 2;
	int diffInc = 1;

	Room rooms[NUM_ROOMS];
};