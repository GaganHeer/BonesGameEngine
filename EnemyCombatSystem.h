#include <time.h>
#include <stdlib.h>
#include <stdexcept>

class EnemyCombatSystem
{
public:
	EnemyCombatSystem(int newHealth, int newAtk, int newXP);

	//Getters
	int getCurrentHealth() {
		return currentHealth;
	}

	int getBaseHealth() {
		return baseHealth;
	}

	int getAtk() {
		return atk;
	}

	int getXP() {
		return XP;
	}

	//Setters
	void setCurrentHealth(int newHealth) {
		currentHealth = newHealth;
	}

	void setBaseHealth(int newHealth) {
		baseHealth = newHealth;
	}

	void setAtk(int newAtk) {
		atk = newAtk;
	}

	void setXP(int newXP) {
		XP = newXP;
	}

	bool isDebuffing();
	int performAtk();
	void takeDmg(int playerAtk);
	bool checkIfDead();
	void resetEnemy();

private:
	int baseHealth;
	int currentHealth;
	int atk;
	int XP;
};
