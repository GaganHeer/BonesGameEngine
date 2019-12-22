#include "VictorySprite.h"
#include "Renderer.h"
#include "Game.h"

VictorySprite::VictorySprite(Game* game)
	:Actor(game), 
	tileNum(0), 
	time(0), 
	ready(true) 
{
	sc = new SpriteComponent(this, 10);
	sc->SetTexture(game->GetRenderer()->GetTexture("Assets/victory-dance-alt/tile0.png"));
}

void VictorySprite::UpdateActor(float deltaTime) {
	time += deltaTime;
	if (time >= .07f) {
		ready = true;
		SetPosition(Vector3(0.0f, 50.0f, 0.0f));
		SetScale(3.f);
		tileNum++;
		if (tileNum > 137)
			tileNum = 0;
		
		//string temp = "";
		//if (tileNum <= 99) {
		//	temp.append("0");
		//	if (tileNum <= 9) {
		//		temp.append("0");
		//	}
		//}

		string file = "Assets/victory-dance-alt/tile" + std::to_string(tileNum);
		file.append(".png");

		sc->SetTexture(this->GetGame()->GetRenderer()->GetTexture(file));
		time = 0;
	}
}