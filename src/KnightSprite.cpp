#include "KnightSprite.h"
#include "Renderer.h"
#include "Game.h"

KnightSprite::KnightSprite(Game* game)
	:Actor(game),
	tileNum(0),
	time(0),
	state(Idle),
	ready(true)
{
	// Create a sprite component
	sc = new SpriteComponent(this, 150);
	sc->SetTexture(game->GetRenderer()->GetTexture("Assets/knight-idle/tile0.png"));
}

void KnightSprite::UpdateActor(float deltaTime)
{
	time += deltaTime;
	if (time >= .15) {
		if (state == Idle) {
			ready = true;
			SetPosition(Vector3(350.0f, 50.0, 0.0f));
			SetScale(3.f);
			tileNum++;
			if (tileNum > 3)
				tileNum = 0;
			string file = "Assets/knight-idle/tile" + std::to_string(tileNum);
			file.append(".png");

			sc->SetTexture(this->GetGame()->GetRenderer()->GetTexture(file));
			time = 0;
		}
		else if (state == Attacking) {
			SetPosition(Vector3(250.0f, 125.0, 0.0f));
			SetScale(4.f);
			tileNum++;
			if (tileNum > 9) {
				tileNum = 9;
				ready = true;
			}
			string file = "Assets/knight-attack/tile" + std::to_string(tileNum);
			file.append(".png");

			sc->SetTexture(this->GetGame()->GetRenderer()->GetTexture(file));
		}
		else if (state == Dodging) {
			tileNum++;
			if (tileNum > 6) {
				tileNum = 6;
				ready = true;
			}
			string file = "Assets/knight-dodge/tile" + std::to_string(tileNum);
			file.append(".png");

			sc->SetTexture(this->GetGame()->GetRenderer()->GetTexture(file));
		}
		else if (state == Dying) {
			tileNum++;
			if (tileNum > 8) {
				tileNum = 8;
				ready = true;
			}
			string file = "Assets/knight-dead/tile" + std::to_string(tileNum);
			file.append(".png");

			sc->SetTexture(this->GetGame()->GetRenderer()->GetTexture(file));
		}
		time = 0;
	}
	if (ready && (state == Attacking || state == Dodging))
		SwitchState(Idle);
}

void KnightSprite::SwitchState(SpriteStates s) {
	state = s;
	tileNum = 0;
	ready = false;
}