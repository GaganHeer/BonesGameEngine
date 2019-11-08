#include "EnemyCombatSystem.h"

EnemyCombatSystem::EnemyCombatSystem(int newHealth, int newAtk, int newXP) {
	health = newHealth;
	atk = newAtk;
	XP = newXP;
}

//enemy has a 25% chance to debuff the player
bool EnemyCombatSystem::isDebuffing() {
	int randomNum;
	srand(time(NULL));
	randomNum = rand() % 4;
	if (randomNum == 0) {
		return true;
	} else {
		return false;
	}
}

//enemy chooses an action to use: either debuffing the player or atking the player
int EnemyCombatSystem::performAtk() {
	if (isDebuffing()) {
		printf("Enemy used Debuff\n");
		return 0;
	} else {
		printf("Enemy used Atk\n");
		return getAtk();
	}
}

//reduce enemy health by dmg done
void EnemyCombatSystem::takeDmg(int playerAtk) {
	setHealth(getHealth() - playerAtk);
}

//return true if dead or false if alive
bool EnemyCombatSystem::checkIfDead() {
	if (getHealth() <= 0) {
		return true;
	} else {
		return false;
	}
}