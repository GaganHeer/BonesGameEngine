#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <SDL.h>
#include "Math.h"


#undef main

class Game
{
public:
	Game();
	//Initialize game
	bool Initialize();
	//Runs game loop
	void RunLoop();
	//Shutdown game
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	//Renderer
	class Renderer* GetRenderer() { return renderer; }
private:
	//Helper functions
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	std::vector<class Actor*> actors;
	std::vector<class Actor*> pendingActors;

	class Renderer* renderer;

	Uint32 ticksCount;
	bool isRunning;
	bool updatingActors;

	class CameraActor* cameraActor;
};

