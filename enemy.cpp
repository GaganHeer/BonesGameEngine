#include "enemy.h"


enemy::enemy() {
	//do something
}

enemy::enemy(int x, int y) {
	_curX = x;
	_curY = y;
}

enemy::~enemy() {

}

void enemy::updateEnemy(int x, int y) {
	_curX = x;
	_curY = y;
}

int* enemy::getPosition() {
	int size[2] = { _curX, _curY };
	return size;
}

void enemy::setup(CubeActor* enemy) {
	//cubeactor object = parameter of enemy (cubeactor)
	cout << " SETTING UP WITH : " << enemy << endl;
	cubeActor = enemy;
}

CubeActor* enemy::getActor() {
	return(cubeActor);
}

void enemy::update() {
	int direction = rand() % 4 + (0);
	float x = 0;
	float y = 0;

	//cout << "DIRECTION : " << direction << endl;
	if (direction == 1) { // North
		y = 100.0f;
	}
	else if (direction == 2) { // eAST
		x = 100.0f;
	}
	else if (direction == 3) { // South
		y = -100.0f;
	}
	else if (direction == 4) { // West
		x = -100.0f;
	}

	//cubeActor->SetPosition(cubeActor->GetPosition() + Vector3(x, y, .0f));
	//Vector3 pos = cubeActor->GetPosition() + Vector3(x, y, .0f);
	//cubeActor->SetPosition(pos);
	//cout << "SETTING UP POSITIONG WITH: " << cubeActor << endl;
	//cubeActor->SetPosition(Vector3(100.0f, 200.0f, .0f));
	//cout << " I'm supposed to move " << endl;
}
