#include "EnemyMoveComponent.h"
#include "Actor.h"

EnemyMoveComponent::EnemyMoveComponent(class Actor* newOwner, int updateOrder)
	:Component(newOwner, updateOrder),
	movingUp(false),
	movingDown(false),
	movingLeft(false),
	movingRight(false),
	spotted(false),
	speed(20.0f)
{
	game = newOwner->GetGame();
}

void EnemyMoveComponent::Update(float deltaTime) {
	if (move && moveable && !movingUp && !movingDown && !movingLeft && !movingRight) {
		MoveEnemy();
	}
	UpdateMovement();
}

void EnemyMoveComponent::MoveEnemy()
{
	map2D = game->GetMap2D();
	Vector3 epos = owner->GetPosition();
	int row = (int)(epos.x / 100);
	int col = (int)(epos.y / 100);
	pair<int, int> p = game->EnemySpotted(row, col);
	if (p.first > -1 && p.second > -1) {
		spotted = true;
	}
	else {
		spotted = false;
	}

	if (!spotted) {
		EnemyWander();
	}
	else {
		int x = 0;
		int y = 0;

		if (p.first > 4 && p.second == 4) {
			x = -1;
		}
		else if (p.first < 4 && p.second == 4) {
			x = 1;
		}
		else if (p.first == 4 && p.second > 4) {
			y = 1;
		}
		else if (p.first == 4 && p.second < 4) {
			y = -1;
		}
		int enem_row = (int)(owner->GetPosition().x / 100);
		int enem_col = (int)(owner->GetPosition().y / 100);
		dest_row = enem_row + x;
		dest_col = enem_col + y;
		game->SetWalkable(enem_row, enem_col);
		if (game->IsWalkable(dest_row, dest_col) == 1 || game->IsWalkable(dest_row, dest_col) == 3) {
			// Set enemy pos here so its blocked for other enemies
			if (game->IsWalkable(dest_row, dest_col) == 1) {
				game->SetEnemyMapPos(dest_row, dest_col);
			}
			if (x == -1) {
				movingLeft = true;
				dest = owner->GetPosition().x - 100;
			}
			else if (x == 1) {
				movingRight = true;
				dest = owner->GetPosition().x + 100;
			}
			else if (y == -1) {
				movingDown = true;
				dest = owner->GetPosition().y - 100;
			}
			else if (y == 1) {
				movingUp = true;
				dest = owner->GetPosition().y + 100;
			}
		}
	}
}

void EnemyMoveComponent::UpdateMovement()
{
	if (movingUp) {
		Vector3 pos = owner->GetPosition();
		//cout << " CURRENT POSITION: " << pos.y << endl;

		pos.y += speed;
		owner->SetPosition(pos);
		if (pos.y > dest) { //dest = current pos + 100
			movingUp = false;
			pos.y = dest;
			owner->SetPosition(pos);
			if (game->IsWalkable(dest_row, dest_col) == 3) {
				game->SetEnemyCollision(true);
			}
		}
	}
	if (movingRight) {
		Vector3 pos = owner->GetPosition();
		//cout << " CURRENT POSITION: " << pos.x << endl;

		pos.x += speed;
		owner->SetPosition(pos);
		if (pos.x > dest) { //dest = current pos + 100
			movingRight = false;
			pos.x = dest;
			owner->SetPosition(pos);
			if (game->IsWalkable(dest_row, dest_col) == 3) {
				game->SetEnemyCollision(true);
			}
		}
	}
	if (movingDown) {
		Vector3 pos = owner->GetPosition();
		//cout << " CURRENT POSITION: " << pos.y << endl;

		pos.y -= speed;
		owner->SetPosition(pos);
		if (pos.y < dest) { //dest = current pos + 100
			movingDown = false;
			pos.y = dest;
			owner->SetPosition(pos);
			if (game->IsWalkable(dest_row, dest_col) == 3) {
				game->SetEnemyCollision(true);
			}
		}
	}
	if (movingLeft) {
		Vector3 pos = owner->GetPosition();
		//cout << " CURRENT POSITION: " << pos.x << endl;

		pos.x -= speed;
		owner->SetPosition(pos);
		if (pos.x < dest) { //dest = current pos + 100
			movingLeft = false;
			pos.x = dest;
			owner->SetPosition(pos);
			if (game->IsWalkable(dest_row, dest_col) == 3) {
				game->SetEnemyCollision(true);
			}
		}
	}
}

void EnemyMoveComponent::EnemyWander()
{
	int moveChance = rand() % 100;
	if (moveChance < 25) {
		Vector3 pos2;
		int x = 0;
		int y = 0;
		int enem_row = 0;
		int enem_col = 0;
		int tempX = 0;
		int tempY = 0;
		int direction = rand() % 4 + (0);
		enem_row = (int)(owner->GetPosition().x / 100);
		enem_col = (int)(owner->GetPosition().y / 100);
		direction = rand() % 4 + (0);
		if (direction == 0) { // North
			y = 1;
		}
		else if (direction == 1) { // East
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
			if (game->IsWalkable(enem_row, enem_col) != 3) {
				if (direction == 0) {
					movingUp = true;
					dest = owner->GetPosition().y + 100;
				}
				if (direction == 1) {
					movingRight = true;
					dest = owner->GetPosition().x + 100;
				}
				if (direction == 2) {
					movingDown = true;
					dest = owner->GetPosition().y - 100;
				}
				if (direction == 3) {
					movingLeft = true;
					dest = owner->GetPosition().x - 100;
				}
				game->SetWalkable(enem_row, enem_col);
				game->SetEnemyMapPos(tempX, tempY);
			}
		}
		else if (game->IsWalkable(tempX, tempY) == 3) {
			cout << "Player Encountered " << endl;
			Vector3 pos = owner->GetPosition() + Vector3(x * 100, y * 100, .0f);
			owner->SetPosition(pos);
			game->SetEnemyCollision(true);
		}
		SetMove(false);
	}
}