#include "Game.h"
#include <algorithm>
#include "SDL_image.h"
#include "Actor.h"

Game::Game()
	:window(nullptr),
	renderer(nullptr),
	ticksCount(0),
	isRunning(true),
	updatingActors(false)
{

}

bool Game::Initialize() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	window = SDL_CreateWindow(
		"Bones Engine",	//Window Title
		100,			//Top left x-coordinates of window
		100,			//Top left y-coordinates of window
		1024,			//Width of window
		768,			//Height of window
		0				//Flags
	);

	if (!window) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	renderer = SDL_CreateRenderer(
		window,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!renderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	LoadData();

	ticksCount = SDL_GetTicks();

	return true;
}

void Game::RunLoop() {
	while (isRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE]) {
		isRunning = false;
	}
}

void Game::UpdateGame() {
	//Compute delta time
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - ticksCount) / 1000.0f;

	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}

	ticksCount = SDL_GetTicks();

	//Update all actors
	updatingActors = true;

	for (auto actor : actors) {
		actor->Update(deltaTime);
	}

	updatingActors = false;
	
	//Move pending actors to actors
	for (auto pending : pendingActors) {
		actors.emplace_back(pending);
	}
	pendingActors.clear();

	//Add dead actors to a temp vector
	std::vector<Actor*> deadActors;

	for (auto actor : actors) {
		if (actor->GetState() == Actor::Dead) {
			deadActors.emplace_back(actor);
		}
	}

	//Delete dead actors
	for (auto actor : deadActors) {
		delete actor;
	}
}

void Game::GenerateOutput() {
	SDL_SetRenderDrawColor(
		renderer,
		100,
		100,
		100,
		255
	);

	SDL_RenderClear(renderer);

	SDL_RenderPresent(renderer);
}

void Game::LoadData() {

}

void Game::UnloadData() {
	while (!actors.empty()) {
		delete actors.back();
	}

	for (auto i : textures) {
		SDL_DestroyTexture(i.second);
	}
	textures.clear();
}

SDL_Texture* Game::GetTexture(const std::string& fileName) {
	SDL_Texture* tex = nullptr;

	auto iter = textures.find(fileName);
	if (iter != textures.end()) {
		tex = iter->second;
	}
	else {
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf) {
			SDL_Log("Failed to laod texture file %s", fileName.c_str());
			return nullptr;
		}

		tex = SDL_CreateTextureFromSurface(renderer, surf);
		SDL_FreeSurface(surf);
		if (!tex) {
			SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
			return nullptr;
		}

		textures.emplace(fileName.c_str(), tex);
	}
	return tex;
}

void Game::AddActor(Actor* actor) {
	if (updatingActors) {
		pendingActors.emplace_back(actor);
	}
	else {
		actors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor) {
	auto iter = std::find(pendingActors.begin(), pendingActors.end(), actor);
	if (iter != pendingActors.end()) {
		std::iter_swap(iter, pendingActors.end() - 1);
		pendingActors.pop_back();
	}

	iter = std::find(actors.begin(), actors.end(), actor);
	if (iter != actors.end()) {
		std::iter_swap(iter, actors.end() - 1);
		actors.pop_back();
	}
}

void Game::Shutdown() {
	UnloadData();
	IMG_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}