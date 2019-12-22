#pragma once
class LevelUpSystem
{
public:

	LevelUpSystem();

	//Setters
	void setCurrentLevel(int newLevel) {
		currentLevel = newLevel;
	}

	void setCurrentXP(int newXP) {
		currentXP = newXP;
	}

	void setRequiredXP(int newXP) {
		requiredXP = newXP;
	}


	//Getters
	int getCurrentLevel() {
		return currentLevel;
	}

	int getCurrentXP() {
		return currentXP;
	}

	int getRequiredXP() {
		return requiredXP;
	}

	void checkLevelUp();
	bool addXP(int XPAmt);

private:
	int currentLevel;
	int currentXP;
	int requiredXP;
};