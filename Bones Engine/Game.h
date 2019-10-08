#pragma once
#include "SDL.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "Texture.h"
#include "SpriteComponent.h"

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

	std::unordered_map<std::string, class Texture*> textures;

	std::vector<class Actor*> actors;
	std::vector<class Actor*> pendingActors;
	std::vector<class SpriteComponent*> mSprites;

	class Shader* mSpriteShader;
	class VertexArray* mSpriteVerts;

	SDL_Window* window;
	SDL_GLContext context;
	Uint32 ticksCount;
	bool isRunning;
	bool updatingActors;
};

