#pragma once
#include <stdlib.h> 
#include <math.h>
#include <iostream>
#include <time.h>
#include <ctime>
#include <string.h>

using namespace std;

class enemy
{
public:
	enemy();
	enemy(int x, int y);
	~enemy();

	void updateEnemy(int x, int y);
	int* getPosition();

private:
	int _curX;
	int _curY;
};

