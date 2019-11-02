#pragma once
#include <stdlib.h> 
#include <math.h>
#include <iostream>
#include <time.h>
#include <ctime>
#include <string.h>
#include <vector>
#include "enemy.h"

using namespace std;

class Room
{
public:
	Room();
	Room(int width, int height, int variance, int maxEnemies);
	~Room();

	void updateRoom(int width, int height, int variance, int maxEnemies);
	bool generate(bool lastDoor);
	int * getParameters();
	vector<enemy> getEnemies();
	void setStart();
	void setEnd(int diffInc);

private:
	int _height;
	int _width;
	int _variance;
	int _varX;
	int _varY;
	int _nextRoomCorridor;

	bool _exit; //returns true if the exit door is north. False if the exit door is east.
	bool _entry; //returns true if the entry door is south. False if the entry door is west.
	bool _isStart; //returns true if the room is the starting room
	bool _isEnd; //returns true if the room is the last room

	int _entryDoor;
	int _exitDoor;

	int _stairX;
	int _stairY;

	int _maxEnemies;
	int _diffInc;

	vector<enemy> _enemies;

	void _debug();
};
