#include "Generator.h"
// ProceduralGeneration.cpp : This file contains the 'main' function. Program execution begins and ends there.
// x is width
// y is length
// variance determines maximum and minimum size difference for width and height.
// The first room will always be the starting room
// The last room will always have the exit. 
// Each room will be connected by corridors. 

Generator::Generator()
{
	rooms[0].setStart();
	rooms[NUM_ROOMS - 1].setEnd(diffInc);

	//rooms[sizeof(rooms)]
	for (int i = 0; i < NUM_ROOMS; i++)
	{
		rooms[i].updateRoom(x, y, variance, maxEnemies);
	}
}

Generator::~Generator()
{

}

void Generator::setParams(int newX, int newY, int newVariance) {
	x = newX;
	y = newY;
	variance = newVariance;
}

//Generates the Rooms. Stores room objects in an array.
void Generator::generate() {
	srand(time(NULL));

	bool temp = false;

	for (int i = 0; i < NUM_ROOMS; i++) {
		temp = rooms[i].generate(temp);
	}
	//prints room parameters
	for (int n = 0; n < NUM_ROOMS; n++) {
		int* temp = rooms[n].getParameters();
		cout << "Room: " << n << " -> " << "[" << temp[0] << ", " << temp[1] << "]";
		printf("\n");
	}
}



