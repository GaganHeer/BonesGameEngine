#pragma once
#include "BossSprite.h"
#include "Renderer.h"
#include "Game.h"

BossSprite::BossSprite(Game* game)
	:Actor(game),
	tileNum(0),
	time(0),
	state(Idle),
	ready(true)
{
	// Create a sprite component
	sc = new SpriteComponent(this, 150);
	sc->SetTexture(game->GetRenderer()->GetTexture("Assets/boss-idle/tile0.png"));
}

void BossSprite::UpdateActor(float deltaTime)
{
	time += deltaTime;
	if (time >= .1) {
		if (state == Idle) {
			ready = true;
			SetPosition(Vector3(300.0f, 50.0, 0.0f));
			tileNum++;
			if (tileNum > 14)
				tileNum = 0;
			string file = "Assets/boss-idle/tile" + std::to_string(tileNum);
			file.append(".png");

			sc->SetTexture(this->GetGame()->GetRenderer()->GetTexture(file));
		}
		else if (state == Attacking) {
			SetPosition(Vector3(150.0f, 50.0, 0.0f));
			tileNum++;
			if (tileNum > 21) {
				tileNum = 21;
				ready = true;
			}
			string file = "Assets/boss-attack/tile" + std::to_string(tileNum);
			file.append(".png");

			sc->SetTexture(this->GetGame()->GetRenderer()->GetTexture(file));
		}
		else if (state == Dodging) {
			tileNum++;
			if (tileNum > 6) {
				tileNum = 6;
				ready = true;
			}
			string file = "Assets/boss-dodge/tile" + std::to_string(tileNum);
			file.append(".png");

			sc->SetTexture(this->GetGame()->GetRenderer()->GetTexture(file));
		}
		else if (state == Dying) {
			tileNum++;
			if (tileNum > 14) {
				tileNum = 14;
				ready = true;
			}
			string file = "Assets/boss-dead/tile" + std::to_string(tileNum);
			file.append(".png");

			sc->SetTexture(this->GetGame()->GetRenderer()->GetTexture(file));
		}
		time = 0;
	}
	if (ready && (state == Attacking || state == Dodging))
		SwitchState(Idle);
}

void BossSprite::SwitchState(SpriteStates s) {
	state = s;
	tileNum = 0;
	ready = false;
}