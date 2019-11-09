#include "room.h"

//room::room(int width, int height) {
//	//do somethging
//}

Room::Room()
{
	cout << " PRINTING STUFF ";
	_isStart = false;
	_isEnd = false;
	_entry = NULL;
	_exit = NULL;
	_entryDoor = NULL;
	_exitDoor = NULL;
	_diffInc = 0;
	_nextRoomCorridor = 0;
	_stairX = 0;
	_stairY = 0;
}

Room::Room(int width, int height, int variance, int maxEnemies)
{
	Room();
	_width = width;
	_height = height;
	_variance = variance;
	_maxEnemies = maxEnemies + 1;
}

Room::~Room()
{
	// dealloc stuff here. in case you use 'new' or 'malloc' or whatever
}

bool Room::generate(bool lastDoor) {
	int tempVar = (_variance * 2) + 1;

	_varX = rand() % tempVar + (-_variance);
	_varY = rand() % tempVar + (-_variance);

	_width += _varX;
	_height += _varY;

	//Assign door location
	//Determines if entry door is located on the north or east wall. Only fires if the room is not marked as the starting room.
	if (!_isStart) {
		//Makes the entry door the same face from the last rooms exit door. For simplicity. 
		_entry = lastDoor;


		//For entry doors. These doors will be placed either on the south or west walls, and are determined by counting from the left (for south) or from the bottom.
		if (_entry) {
			_entryDoor = rand() % _width + (0);
		}
		else {
			_entryDoor = rand() % _height + (0);
		}
	}
	//Determines if exit door is located on the south or west wall. Only fires if the room is not marked as the last room.
	if (!_isEnd) {
		int NE = rand() % 2 + (0);
		_exit = NE;

		//For exit doors. These doors will be placed either on the north or east walls, and are determined by counting from the left (for south) or from the bottom.
		if (_exit) {
			_exitDoor = rand() % _width + (0);
			_nextRoomCorridor = _height;
		}
		else {
			_exitDoor = rand() % _height + (0);
			_nextRoomCorridor = _width;
		}


	}
	else {
		_stairX = rand() % _width + (0);
		_stairY = rand() % _height + (0);
	}

	//Creates enemies
	if (!_isStart) {
		int enemiesToGenerate = rand() % _maxEnemies + (0);
		enemiesToGenerate += _diffInc;

		enemy setup;
		_enemies.assign(enemiesToGenerate, setup);

		vector<vector<int>> lastPlacements;
		vector<int> assign;
		assign.assign(2, -1);
		lastPlacements.assign(enemiesToGenerate, assign);

		//places the enemy
		for (int i = 0; i < enemiesToGenerate; i++) {
			int enemyX = -1;
			int enemyY = -1;

			while (lastPlacements[i][0] == enemyX && lastPlacements[i][1] == enemyY) {
				enemyX = rand() % _width + (0);
				enemyY = rand() % _height + (0);
			}

			lastPlacements[i][0] = enemyX;
			lastPlacements[i][1] = enemyY;
			_enemies[i].updateEnemy(enemyX, enemyY);
		}
		if (_isEnd) {
			if (_entry) { // if is south
				_enemies.back().updateEnemy(_entryDoor, 0);
			}
			else {
				_enemies.back().updateEnemy(0, _entryDoor);
			}

		}
	}

	_debug(); //Debug function. Uncomment to debug

	return _exit;
}

void Room::setStart() {
	_isStart = true;
}
void Room::setEnd(int diffInc) {
	_isEnd = true;
	_diffInc = diffInc;
}

void Room::updateRoom(int width, int height, int variance, int maxEnemies)
{
	_width = width;
	_height = height;
	_variance = variance;
	_maxEnemies = maxEnemies + 1;
}

int* Room::getParameters() {
	int size[11] = { _width, _height, _entry, _entryDoor, _exit, _exitDoor, _isStart, _isEnd, _stairX, _stairY, _nextRoomCorridor };
	return size;
}

vector<enemy> Room::getEnemies() {
	return _enemies;
}

void Room::_debug() {
	string entryOut;
	string exitOut;
	string fluffOne;
	string fluffTwo;
	string fluffThree;

	if (_entry) {
		entryOut = "South";
		fluffOne = "From the Left";
	}
	else {
		entryOut = "West";
		fluffOne = "From the Bottom";
	}
	if (_exit) {
		exitOut = "North";
		fluffTwo = "From the Left";
		fluffThree = "From north face to south face (bottom up)";
	}
	else {
		exitOut = "East";
		fluffTwo = "From the Bottom";
		fluffThree = "From East face to west face (left to right)";
	}

	cout << " IS START: " << _isStart;
	printf("\n");
	cout << " IS END: " << _isEnd;
	printf("\n\n");

	if (_isEnd) {
		cout << "Stair Location: [" << _stairX << ", " << _stairY << "]";
		printf("\n");
	}

	cout << " ENTRY: " << entryOut;
	printf("\n");
	cout << " EXIT: " << exitOut;
	printf("\n\n");

	cout << " ENTRY LOCATION: " << _entryDoor << " " << fluffOne;
	printf("\n");
	cout << " EXIT LOCATION: " << _exitDoor << " " << fluffTwo;
	printf("\n\n");

	cout << " VARIANCE X: " << _varX;
	printf("\n");
	cout << " VARIANCE Y: " << _varY;
	printf("\n\n");

	cout << " WIDTH X: " << _width;
	printf("\n");
	cout << " HEIGHT Y: " << _height;
	printf("\n");
	cout << " CORRIDOR LENGTH: " << _nextRoomCorridor << " " << fluffThree;
	printf("\n");
	printf("\n");


	cout << "ENEMIES TO GENERATE: " << _enemies.size();
	printf("\n");
	for (int i = 0; i < _enemies.size(); i++) {
		int* posTemp = _enemies[i].getPosition();
		cout << "ENEMY: " << i << " POSITION: [" << posTemp[0] << ", " << posTemp[1] << "]";
		printf("\n");
	}
	printf("\n");



	printf("\n");
	printf("==================");
	printf("\n");
}