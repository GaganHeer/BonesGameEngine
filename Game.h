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

	class CameraTargetActor* GetPlayer() {
		return cameraTargetActor;
	}

	GameState GetState() const {
		return gameState;
	}

	void SetState(GameState state) {
		gameState = state;
	}

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();
	
	std::vector<class Actor*> actors;
	std::vector<class Actor*> pendingActors;

	class InputSystem* inputSystem;
	class AudioEngine* AE;
	class Renderer* renderer;
	Uint32 ticksCount;
	GameState gameState;
	bool isRunning;
	bool updatingActors;

	//Game Specific
	class CameraTargetActor* cameraTargetActor;
	class PlayerCombatSystem* playerCombat;
	class LevelUpSystem* playerLevels;
	class EnemyCombatSystem* enemyCombat;
};
