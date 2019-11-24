#include "PlayerCombatSystem.h"

PlayerCombatSystem::PlayerCombatSystem() {
	baseAtk = 25;
	baseHealth = 100;
	currentAtk = baseAtk;
	currentHealth = baseHealth;
	debuffAmt = 0;
}

//Getters
int PlayerCombatSystem::getBaseAtk() {
	return baseAtk;
}

int PlayerCombatSystem::getBaseHealth() {
	return baseHealth;
}

int PlayerCombatSystem::getCurrentAtk() {
	return currentAtk;
}

int PlayerCombatSystem::getCurrentHealth() {
	return currentHealth;
}

int PlayerCombatSystem::getDebuffAmt() {
	return debuffAmt;
}

//Setters
void PlayerCombatSystem::setBaseAtk(int newAtk) {
	baseAtk = newAtk;
}

void PlayerCombatSystem::setBaseHealth(int newHealth) {
	baseHealth = newHealth;
}

void PlayerCombatSystem::setCurrentAtk(int newAtk) {
	currentAtk = newAtk;
}

void PlayerCombatSystem::setCurrentHealth(int newHealth) {
	currentHealth = newHealth;
}

void PlayerCombatSystem::setDebuffAmt(int newDebuff) {
	debuffAmt = newDebuff;
}

//100% accurate but less dmg then heavy
void PlayerCombatSystem::selectLightAtk() {
	int newAtk = getBaseAtk() - getDebuffAmt();
	setCurrentAtk(newAtk);
}

//60% accurate but twice as much dmg as light
void PlayerCombatSystem::selectHeavyAtk() {
	int randomNum;
	srand(time(NULL));
	randomNum = rand() % 10;

	if (randomNum <= 3) {
		currentAtk = 0;
	}
	else {
		int newAtk = (getBaseAtk() * 2) - getDebuffAmt();
		setCurrentAtk(newAtk);
	}
}

//atkType 0 = light, atkType 1 = heavy
int PlayerCombatSystem::dealDmg(int atkType) {
	if (atkType == 0) {
		selectLightAtk();
		printf("Player used Light Atk\n");
		printf("Player Hit!\n");
	}
	else if (atkType == 1) {
		selectHeavyAtk();
		printf("Player used Heavy Atk\n");
		if (getCurrentAtk() == 0) {
			printf("Player Misses!\n");
		} else {
			printf("Player Hit!\n");
		}
	}
	else {
		throw std::invalid_argument("Atk Type must be set to 0 or 1");
	}
	return getCurrentAtk();
}

//if enemy's atk is 0 it means that the enemy is doing a debuff instead of atking
void PlayerCombatSystem::takeDmg(int enemyAtk) {
	if (enemyAtk == 0) {
		printf("Player is debuffed\n");
		if (!(getCurrentAtk() - getDebuffAmt() <= 5)) {
			setDebuffAmt(getDebuffAmt() + 5);
		}

	}
	else {
		printf("%s%d%s", "Player took ", enemyAtk, " damage\n");
		int health = getCurrentHealth();
		health -= enemyAtk;
		setCurrentHealth(health);
	}
}

//return true if dead or false if alive
bool PlayerCombatSystem::checkIfDead() {
	if (getCurrentHealth() <= 0) {
		return true;
	} else {
		return false;
	}
}

void PlayerCombatSystem::increaseStats() {
	setBaseHealth(getBaseHealth() + 20);
	setBaseAtk(getBaseAtk() + 10);
	//heal the player after leveling
	setCurrentHealth(getBaseHealth());
	setCurrentAtk(getBaseAtk());
}