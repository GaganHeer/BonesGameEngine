#include "Generator.h"
#include "Game.h"
// ProceduralGeneration.cpp : This file contains the 'main' function. Program execution begins and ends there.
// x is width
// y is length
// variance determines maximum and minimum size difference for width and height.
// The first room will always be the starting room
// The last room will always have the exit. 
// Each room will be connected by corridors. 

Generator::Generator(Game* game) :
	game(game)
{
	for (int i = 0; i < NUM_ROOMS; i++) {
		rooms.emplace_back(new Room(game));
	}
	rooms.at(0)->setStart();
	rooms.at(NUM_ROOMS - 1)->setEnd(diffInc);

	//rooms[sizeof(rooms)]
	for (int i = 0; i < NUM_ROOMS; i++)
	{
		rooms.at(i)->updateRoom(x, y, variance, maxEnemies);
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
vector<Room*> Generator::generate() {
	srand(time(NULL));

	bool temp = false;

	for (int i = 0; i < NUM_ROOMS; i++) {
		temp = rooms[i]->generate(temp);
	}
	//prints room parameters
	for (int n = 0; n < NUM_ROOMS; n++) {
		int* returnedParams = rooms[n]->getParameters();
		//cout << "Room: " << n << " -> " << "[" << temp[0] << ", " << temp[1] << "]";
		//printf("\n");
		cout << " CORRIDOR LENGTH TWO: " << returnedParams[10] << endl;
	}

	return rooms;
}

int Generator::getNumRooms() {
	return NUM_ROOMS;
}

//{ _width, _height, _entry, _entryDoor, _exit, _exitDoor, _isStart, _isEnd, _stairX, _stairY, _nextRoomCorridor };

int Generator::getWidth(int i) {
	return rooms[i]->getParameters()[0];
}
int Generator::getHeight(int i) {
	return rooms[i]->getParameters()[1];
}
int Generator::getEntryLocation(int i) {
	return rooms[i]->getParameters()[2];
}
int Generator::getEntryDoor(int i) {
	return rooms[i]->getParameters()[3];
}
int Generator::getExitLocation(int i) {
	return rooms[i]->getParameters()[4];
}
int Generator::getExitDoor(int i) {
	return rooms[i]->getParameters()[5];
}
int Generator::getIsStart(int i) {
	return rooms[i]->getParameters()[6];
}
int Generator::getIsEnd(int i) {
	return rooms[i]->getParameters()[7];
}
int Generator::getStairX() {
	return rooms[NUM_ROOMS - 1]->getParameters()[8];
}
int Generator::getStairY() {
	return rooms[NUM_ROOMS - 1]->getParameters()[9];
}
int Generator::getCorridorLength(int i) {
	return rooms[i]->getParameters()[10];
}
vector<EnemyActor*> Generator::getEnemies(int i) {
	return rooms[i]->getEnemies();
}
