#pragma once
#include <SDL.h>
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"
#include "InputSystem.h"
#include "AudioEngine.h"
#include "PlayerCombatSystem.h"
#include "EnemyCombatSystem.h"
#include "LevelUpSystem.h"
#include "Font.h"
#include "Texture.h"
#include <thread>


class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	enum GameState {
		gameplay,
		paused,
		quit
	};

	class Renderer* GetRenderer() { 
		return renderer; 
	}

	class Skeleton* GetSkeleton(const std::string& fileName);

	class Animation* GetAnimation(const std::string& fileName);

	class CameraTargetActor* GetPlayer() {
		return cameraTargetActor;
	}

	GameState GetState() const {
		return gameState;
	}

	void SetState(GameState state) {
		gameState = state;
	}

	void SetEnemyCollision(bool isCollision) {
		enemyCollision = isCollision;
	}

	bool GetEnemyCollision() {
		return enemyCollision;
	}

	int IsWalkable(int row, int col);
	void Game::SetWalkable(int row, int col);
	void Game::SetEnemyMapPos(int row, int col);
	void Game::SetPlayerMapPos(int row, int col);
	void CombatRound(int atkType);

	vector<Vector3> getEnemies() {
		return enem;
	}

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void CreatePointLights(Actor*& a, Vector3& pos, int z);
	void UnloadData();
	void UnloadSkelAnim();
	void InitFontRenderer();
	void UpdateText(Texture*& fontArea, const std::string& text);
	void CleanupFontAreas();
	
	
	std::vector<class Actor*> actors;
	std::vector<class Actor*> pendingActors;

	// Map of loaded skeletons
	std::unordered_map<std::string, class Skeleton*> skeletons;
	// Map of loaded animations
	std::unordered_map<std::string, class Animation*> anims;

	class InputSystem* inputSystem;
	class AudioEngine* AE;
	Studio::EventInstance* currentAudioInstance;
	class Renderer* renderer;
	class Generator* randGen;
	Uint32 ticksCount;
	int scene;
	GameState gameState;
	bool isRunning;
	bool isLoading;
	bool updatingActors;
	std::vector<class EnemyActor*> enemies;
	std::vector<Vector3> enem;
	std::vector<class Room*> rooms;

	int exit_posX;
	int exit_posY;
	int start_posX;
	int start_posY;

	int** map2D;

	//Game Specific
	class CameraTargetActor* cameraTargetActor;
	class CubeActor* cubeActor;
	class PlayerCombatSystem* playerCombat;
	class LevelUpSystem* playerLevels;
	class EnemyCombatSystem* enemyCombat;
	class EnemyActor* enemyActor;

	Texture* fontEnemyHealth;
	Texture* fontPlayerHealth;
	class HUD* hud;

	bool isReturning;
	bool enemyCollision;
	bool isAttacking;
	
	Vector3 savedPlayerPosition;
	vector<Vector3> saved_enemies;
	vector<Actor*> enems;

	vector<int> numEnemies;

};
