#include <time.h>
#include <stdlib.h>
#include <stdexcept>

class EnemyCombatSystem 
{
public:
	EnemyCombatSystem(int newHealth, int newAtk, int newXP);

	//Getters
	int getHealth() {
		return health;
	}

	int getAtk() {
		return atk;
	}

	int getXP() {
		return XP;
	}

	//Setters
	void setHealth(int newHealth) {
		health = newHealth;
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

private:
	int health;
	int atk;
	int XP;
};
