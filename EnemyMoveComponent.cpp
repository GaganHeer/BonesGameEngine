#include "EnemyMoveComponent.h"
#include "Actor.h"

EnemyMoveComponent::EnemyMoveComponent(class Actor* newOwner, int updateOrder)
	:Component(newOwner, updateOrder)
{
	game = newOwner->GetGame();
}

void EnemyMoveComponent::Update(float deltaTime) {
	if (move && moveable) {
		MoveEnemy();
	}
}

void EnemyMoveComponent::MoveEnemy()
{
	Vector3 pos2;
	int x = 0;
	int y = 0;
	int enem_row = 0;
	int enem_col = 0;
	int tempX = 0;
	int tempY = 0;
	int direction = rand() % 4 + (0);
	//cout << "DIRECTION : " << direction << endl;
	enem_row = (int)(owner->GetPosition().x / 100);
	enem_col = (int)(owner->GetPosition().y / 100);
	direction = rand() % 4 + (0);
	x = 0;
	y = 0;
	//cout << "DIRECTION : " << direction << endl;
	if (direction == 0) { // North
		y = 1;
	}
	else if (direction == 1) { // eAST
		x = 1;
	}
	else if (direction == 2) { // South
		y = -1;
	}
	else if (direction == 3) { // West
		x = -1;
	}

	tempX = enem_row + x;
	tempY = enem_col + y;
	if (game->IsWalkable(tempX, tempY) == 1)
	{
		game->SetWalkable(enem_row, enem_col);

		pos2 = owner->GetPosition() + Vector3(x * 100, y * 100, .0f);
		owner->SetPosition(pos2);
		game->SetEnemyMapPos(tempX, tempY);
		
	}
	else if (game->IsWalkable(tempX, tempY) == 3)
	{
		game->SetWalkable(enem_row, enem_col);

		pos2 = owner->GetPosition() + Vector3(x * 100, y * 100, .0f);
		owner->SetPosition(pos2);
		game->SetEnemyMapPos(tempX, tempY);
		Collided();
	}
	SetMove(false);
}


void EnemyMoveComponent::Collided()
{
	cout << "COLLISION: " << game->GetEnemyCollision() << endl;
	game->SetEnemyCollision(true);
	cout << "COLLISION: " << game->GetEnemyCollision() << endl;
}