#include "SkeletonSprite.h"
#include "Renderer.h"
#include "Game.h"

SkeletonSprite::SkeletonSprite(Game* game)
	:Actor(game),
	tileNum(0),
	time(0),
	state(Idle),
	ready(true)
{
	// Create a sprite component
	sc = new SpriteComponent(this, 150);
	sc->SetTexture(game->GetRenderer()->GetTexture("Assets/skele-idle/tile0.png"));
}

void SkeletonSprite::UpdateActor(float deltaTime)
{
	time += deltaTime;
	if (time >= .1) {
		if (state == Idle) {
			ready = true;
			SetPosition(Vector3(-300.0f, 50.0, 0.0f));
			SetScale(2.f);
			tileNum++;
			if (tileNum > 10)
				tileNum = 0;
			string file = "Assets/skele-idle/tile" + std::to_string(tileNum);
			file.append(".png");

			sc->SetTexture(this->GetGame()->GetRenderer()->GetTexture(file));
			time = 0;
		}
		else if (state == Attacking) {
			SetPosition(Vector3(-275.0f, 70.0, 0.0f));
			SetScale(2.75f);
			tileNum++;
			if (tileNum > 17) {
				tileNum = 17;
				ready = true;
			}
			string file = "Assets/skele-attack/tile" + std::to_string(tileNum);
			file.append(".png");

			sc->SetTexture(this->GetGame()->GetRenderer()->GetTexture(file));
		}
		else if (state == Dying) {
			tileNum++;
			if (tileNum > 14) {
				tileNum = 14;
				ready = true;
			}
			string file = "Assets/skele-dead/tile" + std::to_string(tileNum);
			file.append(".png");

			sc->SetTexture(this->GetGame()->GetRenderer()->GetTexture(file));
		}
		time = 0;
	}
	if (ready && state == Attacking)
	SwitchState(Idle);
}

void SkeletonSprite::SwitchState(SpriteStates s) {
	state = s;
	tileNum = 0;
	ready = false;
}