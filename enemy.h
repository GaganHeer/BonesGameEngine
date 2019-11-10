#pragma once
#include <stdlib.h> 
#include <math.h>
#include <iostream>
#include <time.h>
#include <ctime>
#include <string.h>
#include "CubeActor.h"

using namespace std;

class enemy
{
public:
	enemy();
	enemy(int x, int y);
	~enemy();

	void updateEnemy(int x, int y);
	int* getPosition();
	void setup(CubeActor* enemy);
	void update();
	class CubeActor* cubeActor;
	CubeActor* getActor();

private:
	int _curX;
	int _curY;

};

