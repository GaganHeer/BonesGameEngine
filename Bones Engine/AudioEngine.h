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
	void sfx(const char* GUID);
	void update();
private:

};
