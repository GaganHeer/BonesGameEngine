#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* newOwner, int updateOrder)
	:Component(newOwner, updateOrder),
	horizontalMove(0.0f),
	verticalMove(0.0f),
	movingUp(false),
	movingDown(false),
	movingLeft(false),
	movingRight(false),
	speed(20.0f)
{
	game = newOwner->GetGame();

}

void MoveComponent::Update(float deltaTime){
	if (!movingUp && !movingDown && !movingLeft && !movingRight) {
		if (!Math::NearZero(verticalMove)) {
			Vector3 position = owner->GetPosition();
			int row = (int)(position.x / 100);
			int col = (int)(position.y / 100);

			if (game->IsWalkable(row + verticalMove / 100, col) == 1) { //for tiles
				if (verticalMove > 0) {
					movingUp = true;
				}
				else if (verticalMove < 0) {
					movingDown = true;
				}
				//cout << "Is Walkable" << endl;
				game->SetWalkable(row, col);
				game->SetPlayerMapPos(row + verticalMove / 100, col);

				dest = owner->GetPosition().x + verticalMove;
				dest_pos = owner->GetPosition() + Vector3(verticalMove, .0f, .0f);

				//Vector3 pos = owner->GetPosition() + Vector3(verticalMove, .0f, .0f);
				//owner->SetPosition(pos);
				//cout << "Current row,col: " << pos.x / 100 << " " << pos.y / 100 << endl;
			}
			else if (game->IsWalkable(row + verticalMove / 100, col) == 2) { //for enemies
				//cout << "Enemy Encountered " << endl;
				game->SetWalkable(row, col);
				game->SetPlayerMapPos(row + verticalMove / 100, col);
				Vector3 pos = owner->GetPosition() + Vector3(verticalMove, .0f, .0f);
				owner->SetPosition(pos);

				Collided();
				//do something;
			}
			else if (game->IsWalkable(row + verticalMove / 100, col) == 4) {
				//cout << "Stairs Found" << endl;
				StairsFound();
			}
			else {
				//cout << "Not Walkable" << endl;
			}
		}
		else if (!Math::NearZero(horizontalMove)) {
			Vector3 position = owner->GetPosition();
			int row = (int)(position.x / 100);
			int col = (int)(position.y / 100);

			if (game->IsWalkable(row, col + -horizontalMove / 100) == 1) {
				if (horizontalMove > 0) {
					movingRight = true;
				}
				else if (horizontalMove < 0) {
					movingLeft = true;
				}
				//cout << "Is Walkable" << endl;
				game->SetWalkable(row, col);
				game->SetPlayerMapPos(row, col + -horizontalMove / 100);

				dest = owner->GetPosition().y + (-horizontalMove);
				dest_pos = owner->GetPosition() + Vector3(.0f, -horizontalMove, .0f);

				//Vector3 pos = owner->GetPosition() + Vector3(.0f, -horizontalMove, .0f);
				//owner->SetPosition(pos);
				//cout << "Current row,col: " << pos.x / 100 << " " << pos.y / 100 << endl;
			}
			else if (game->IsWalkable(row, col + -horizontalMove / 100) == 2) {
				//cout << "Enemy Encountered " << endl;
				game->SetWalkable(row, col);
				game->SetPlayerMapPos(row, col + -horizontalMove / 100);
				Vector3 pos = owner->GetPosition() + Vector3(.0f, -horizontalMove, .0f);
				owner->SetPosition(pos);

				Collided();
				//do something;
			}
			else if (game->IsWalkable(row, col + -horizontalMove / 100) == 4) {
				//cout << "Stairs Found" << endl;
				StairsFound();
			}
			else {
				//cout << "Is Not Walkable" << endl;
			}
		}
	}
	UpdatePosition();
}

void MoveComponent::UpdatePosition()
{
	if (movingUp) {
		Vector3 pos = owner->GetPosition();
		pos.x += speed;
		owner->SetPosition(pos);
		if (pos.x > dest) {
			movingUp = false;
			owner->SetPosition(dest_pos);
		}
	}

	if (movingDown) {
		Vector3 pos = owner->GetPosition();
		pos.x -= speed;
		owner->SetPosition(pos);
		if (pos.x < dest) {
			movingDown = false;
			owner->SetPosition(dest_pos);
		}
	}

	if (movingLeft) {
		Vector3 pos = owner->GetPosition();
		pos.y += speed;
		owner->SetPosition(pos);
		if (pos.y > dest) {
			movingLeft = false;
			owner->SetPosition(dest_pos);
		}
	}

	if (movingRight) {
		Vector3 pos = owner->GetPosition();
		pos.y -= speed;
		owner->SetPosition(pos);
		if (pos.y < dest) {
			movingRight = false;
			owner->SetPosition(dest_pos);
		}
	}
}

void MoveComponent::Collided()
{
	//cout << "COLLISION: " << game->GetEnemyCollision() << endl;
	game->SetEnemyCollision(true);
	//cout << "COLLISION: " << game->GetEnemyCollision() << endl;
}

void MoveComponent::StairsFound() {
	game->SetStairCollision(true);
}