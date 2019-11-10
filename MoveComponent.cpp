#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* newOwner, int updateOrder)
	:Component(newOwner, updateOrder),
	horizontalMove(0.0f),
	verticalMove(0.0f)
{
	game = newOwner->GetGame();
	enemies = game->getEnemies();

}

void MoveComponent::Update(float deltaTime){
	if (!Math::NearZero(verticalMove)) {
		Vector3 position = owner->GetPosition();
		int row = (int)(position.x / 100);
		int col = (int)(position.y / 100);

		if (game->IsWalkable(row + verticalMove / 100, col) == 1) {
			cout << "Is Walkable" << endl;
			Vector3 pos = owner->GetPosition() + Vector3(verticalMove, .0f, .0f);
			owner->SetPosition(pos);
			cout << "Current row,col: " << pos.x / 100 << " " << pos.y / 100 << endl;
		}
		else if (game->IsWalkable(row + verticalMove / 100, col) == 2) {
			cout << "Enemy Encountered " << endl;
			Vector3 pos = owner->GetPosition() + Vector3(verticalMove, .0f, .0f);
			owner->SetPosition(pos);
			cout << "COLLISION: " << game->GetEnemyCollision() << endl;
			game->SetEnemyCollision(true);
			cout << "COLLISION: " << game->GetEnemyCollision() << endl;
			//do something;
		}
		else {
			cout << "Not Walkable" << endl;
		}
		MoveEnemy();
	}
	else if (!Math::NearZero(horizontalMove)) {
		Vector3 position = owner->GetPosition();
		int row = (int)(position.x / 100);
		int col = (int)(position.y / 100);

		if (game->IsWalkable(row, col + -horizontalMove / 100) == 1) {
			cout << "Is Walkable" << endl;
			Vector3 pos = owner->GetPosition() + Vector3(.0f, -horizontalMove, .0f);
			owner->SetPosition(pos);
			cout << "Current row,col: " << pos.x / 100 << " " << pos.y / 100 << endl;
		}
		else if (game->IsWalkable(row, col + -horizontalMove / 100) == 2) {
			cout << "Enemy Encountered " << endl;
			Vector3 pos = owner->GetPosition() + Vector3(.0f, -horizontalMove, .0f);
			owner->SetPosition(pos);
			cout << "COLLISION: " << game->GetEnemyCollision() << endl;
			game->SetEnemyCollision(true);
			cout << "COLLISION: " << game->GetEnemyCollision() << endl;
			//do something;
		}
		else {
			cout << "Is Not Walkable" << endl;
		}
		MoveEnemy();
	}
}

void MoveComponent::MoveEnemy()
{
	Vector3 cubePos;
	Vector3 pos2;
	int x = 0;
	int y = 0;
	int enem_row = 0;
	int enem_col = 0;
	int tempX = 0;
	int tempY = 0;
	//Move each cube actor after player moves
	for (CubeActor* actor : enemies) {
		cubePos = actor->GetPosition();
		enem_row = (int)(cubePos.x / 100);
		enem_col = (int)(cubePos.y / 100);
		int direction = rand() % 4 + (0);
		x = 0;
		y = 0;
		//cout << "DIRECTION : " << direction << endl;
		if (direction == 1) { // North
			y = 1;
		}
		else if (direction == 2) { // eAST
			x = 1;
		}
		else if (direction == 3) { // South
			y = -1;
		}
		else if (direction == 4) { // West
			x = -1;
		}

		//Vector3 pos = actor->GetPosition() + Vector3(x, .0f, .0f);
		tempX = enem_row + x;
		tempY = enem_col + y;
		if (game->IsWalkable(tempX, tempY) == 1) {
			game->SetWalkable(enem_row, enem_col);
			pos2 = actor->GetPosition() + Vector3(x * 100, y * 100, .0f);
			actor->SetPosition(pos2);
			game->SetEnemyMapPos(tempX, tempY);
		}
	}
}
