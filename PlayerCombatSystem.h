#include <time.h>
#include <stdlib.h>
#include <stdexcept>

class PlayerCombatSystem
{
public:
	PlayerCombatSystem();

	//Getters
	int getBaseAtk();
	int getBaseHealth();
	int getCurrentAtk();
	int getCurrentHealth();
	int getDebuffAmt();

	//Setters
	void setBaseAtk(int newAtk);
	void setBaseHealth(int newHealth);
	void setCurrentAtk(int newAtk);
	void setCurrentHealth(int newHealth);
	void setDebuffAmt(int newDebuff);

	void selectHeavyAtk();
	void selectLightAtk();
	int dealDmg(int atkType);
	void takeDmg(int enemyAtk);
	bool checkIfDead();
	void increaseStats();

private:
	int baseAtk;
	int baseHealth;
	int currentAtk;
	int currentHealth;
	int debuffAmt;
};