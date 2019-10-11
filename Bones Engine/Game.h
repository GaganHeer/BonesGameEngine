#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <GL/glew.h>
#include <SDL.h>
#include "Texture.h"
#include "SpriteComponent.h"
#include "Math.h"
#include "Vertex.h"
#include "SDL_image.h"
#include "Actor.h"


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

	// 2D Sprites
	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	//SDL_Texture* GetTexture(const std::string& fileName);
	class Texture* GetTexture(const std::string& fileName);
private:
	//Helper functions
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	bool LoadShaders();
	void LoadData();
	void UnloadData();
	void CreateSpriteVerts();

	std::unordered_map<std::string, class Texture*> textures;

	std::vector<class Actor*> actors;
	std::vector<class Actor*> pendingActors;
	std::vector<class SpriteComponent*> mSprites;

	class Shader* spriteShader;
	class Vertex* spriteVerts;

	SDL_Window* window;
	SDL_GLContext context;
	Uint32 ticksCount;
	bool isRunning;
	bool updatingActors;
};

