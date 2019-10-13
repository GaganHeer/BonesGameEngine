#pragma once

#include <GL/glew.h>
#include <GL/GL.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <SDL.h>
#include "Math.h"
#include <algorithm>
#include "Renderer.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "CameraActor.h"
#include "InputSystem.h"
#include "AudioEngine.h"
#include "thread"
#include "GUI.h"

#undef main

class Game
{
public:
	Game(int argc, char** argv);
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
	
	void InitGUI();
	void Quit_GUI();

private:
	//Helper functions
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
	bool isRunning;
	bool updatingActors;

	class CameraActor* cameraActor;
	GUI* gui;

	int _argc;
	char** _argv;
};

