#pragma once

#include "fmod_studio.hpp"
#include "fmod.h"
#include "fmod_errors.h"
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <iostream>
#include <Windows.h>

using namespace std;
using namespace FMOD;

class AudioEngine {
public:
	void setup();
	void update();

	void testLooping();
	void stopAudio(Studio::EventInstance* inst);
	Studio::EventInstance* startDungeonBGM();
	Studio::EventInstance* startFightBGM();
	Studio::EventInstance* startBossBGM();
	Studio::EventInstance* startWinBGM();
	Studio::EventInstance* startWinAltBGM();
	Studio::EventInstance* startLoseBGM();
	void playerAtk();
	void enemyAtk();
	void enemyDeath();



private:

};
