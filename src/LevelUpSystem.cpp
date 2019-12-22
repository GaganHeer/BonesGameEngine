#include "LevelUpSystem.h"

LevelUpSystem::LevelUpSystem()
{
	currentLevel = 1;
	currentXP = 0;
	requiredXP = 100;
}

//Assumes that user can only level up a max of once per battle and that XP requirements double after leveling
void LevelUpSystem::checkLevelUp() {
	setCurrentLevel(getCurrentLevel() + 1);
	setRequiredXP(getRequiredXP() * 2);
	setCurrentXP(0);
}

bool LevelUpSystem::addXP(int XPAmt) {
	setCurrentXP(getCurrentXP() + XPAmt);
	if (getCurrentXP() >= getRequiredXP()) {
		checkLevelUp();
		return true;
	} else {
		return false;
	}
	
}
