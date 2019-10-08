#include "Game.h"
#include <algorithm>
#include "SDL_image.h"
#include "Actor.h"
#include <GL/glew.h>
#include "Texture.h"

Game::Game()
	:window(nullptr),
	context(nullptr),
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

	//Use OpenGL Core
	SDL_GL_SetAttribute(
		SDL_GL_CONTEXT_PROFILE_MASK,
		SDL_GL_CONTEXT_PROFILE_CORE
	);

	//OpenGL v3.3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	//Colour buffer
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	//Double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	//Force OpenGL to use hardware acceleration
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	window = SDL_CreateWindow(
		"Bones Engine",	//Window Title
		100,			//Top left x-coordinates of window
		100,			//Top left y-coordinates of window
		1024,			//Width of window
		768,			//Height of window
		SDL_WINDOW_OPENGL				//Flags
	);

	if (!window) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	context = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		SDL_Log("Failed to initialize GLEW.");
		return false;
	}
	
	glGetError();

	if (!LoadShaders()) {
		SDL_Log("Failed to load shaders");
		return false;
	}

	glClearColor(0.75f, .25f, .25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	SDL_GL_SwapWindow(window);

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
	glClearColor(0.75f, 0.25f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SDL_GL_SwapWindow(window);
}

bool Game::LoadShaders() {
	float width = 1024.0f;
	float height = 768.0f;
	Eigen::Matrix4f viewProj;
	viewProj << 2.0f / width, 0.0f, 0.0f, 0.0f,
				0.0f, 2.0f / height, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 1.0f;
	return true;
}

void Game::LoadData() {

}

void Game::UnloadData() {
	while (!actors.empty()) {
		delete actors.back();
	}

	/*	for (auto i : textures) {
		SDL_DestroyTexture(i.second);
	}
	textures.clear();
	*/
	// Destroy textures
	for (auto i : textures)
	{
		i.second->Unload();
		delete i.second;
	}
	textures.clear();

}

/*SDL_Texture* Game::GetTexture(const std::string& fileName) {
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
} */

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

void Game::AddSprite(SpriteComponent* sprite)
{
	// Find the insertion point in the sorted vector
	// (The first element with a higher draw order than me)
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (;
		iter != mSprites.end();
		++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	// Inserts element before position of iterator
	mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}
void Game::Shutdown() {
	UnloadData();
	IMG_Quit();
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

Texture* Game::GetTexture(const std::string& fileName)
{
	Texture* tex = nullptr;
	auto iter = textures.find(fileName);
	if (iter != textures.end())
	{
		tex = iter->second;
	}
	else
	{
		tex = new Texture();
		if (tex->Load(fileName))
		{
			textures.emplace(fileName, tex);
		}
		else
		{
			delete tex;
			tex = nullptr;
		}
	}
	return tex;
}