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
	int size[2] = {_curX, _curY};
	return size;
}