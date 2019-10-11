#include "Game.h"


Game::Game()
	:window(nullptr),
	spriteShader(nullptr),
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

	//2D
	//CreateSpriteVerts();
	
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

	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	if (keyState[SDL_SCANCODE_ESCAPE]) {
		isRunning = false;
	}

	updatingActors = true;
	for (auto actor : actors) {
		actor->ProcessInput(keyState);
	}
	updatingActors = false;

	/*
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
	*/
	

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


	// For 2D Rendering

	// Draw all sprite components and shader
	/*
	spriteShader->SetActive();
	spriteVerts->SetActive();
	for (auto sprite : mSprites)
	{
		sprite->Draw(spriteShader);
	}
	SDL_GL_SwapWindow(window);
	*/
	
}

bool Game::LoadShaders() {
	/*
	spriteShader = new Shader();
	if (!spriteShader->Load("Shaders/Shader.vert", "Shaders/Shader.frag")) {
		return false;
	}
	*/

	//spriteShader->SetActive();

	Eigen::Matrix4f viewProj;
	viewProj = Math::CreateSimpleViewProj4f(1024.0f, 768.0f);

	//spriteShader->SetMatrixUniform("uViewProj", viewProj);
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
	// Insertion point in the sorted vector
	int drawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();

	for (;iter != mSprites.end(); ++iter) {
		if (drawOrder < (*iter)->GetDrawOrder()) {
			break;
		}
	}

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

	// 2D 
	/*
	UnloadData();
	delete spriteVerts;
	spriteShader->Unload();
	delete spriteShader;
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	*/
}

void Game::CreateSpriteVerts()
{
	float vertices[] = {
		-0.5f,  0.5f, 0.f, 0.f, 0.f, // top left
		 0.5f,  0.5f, 0.f, 1.f, 0.f, // top right
		 0.5f, -0.5f, 0.f, 1.f, 1.f, // bottom right
		-0.5f, -0.5f, 0.f, 0.f, 1.f  // bottom left
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	spriteVerts = new Vertex(vertices, 4, indices, 6);
}