#include "Game.h"

using namespace std;

Game::Game(int argc, char** argv)
	:renderer(nullptr),
	isRunning(true),
	updatingActors(false)
{
	_argc = argc;
	_argv = argv;

	inputSystem = new InputSystem();
	AE = new AudioEngine();
}

bool Game::Initialize() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	if (!inputSystem->Initialize())
	{
		SDL_Log("Failed to initialize input system");
		return false;
	}

	//Create Renderer
	renderer = new Renderer(this);
	if (!renderer->Initialize(1024.0f, 768.0f)) {
		SDL_Log("Failed to initialize renderer");
		delete renderer;
		renderer = nullptr;
		return false;
	}

	AE->setup();
	AE->sfx("{8a6a04bd-f459-4efe-9b9f-5b2bd9969d8c}");
	
	LoadData();

	ticksCount = SDL_GetTicks();

	InitGUI();

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
	//const Uint8* state = SDL_GetKeyboardState(NULL);
	inputSystem->BeforeUpdate();
	const InputState& state = inputSystem->GetState();

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			/*case SDL_QUIT:
				isRunning = false;
				break;*/

		case SDL_KEYDOWN:
			//Quit Game
			if (state.Keyboard.GetKeyState(SDL_SCANCODE_ESCAPE) == ButtonState::Pressed)
			{
				isRunning = false;
			}

			//Button just pressed down
			if (state.Keyboard.GetKeyState(SDL_SCANCODE_W) == ButtonState::Pressed)
			{
				printf("W Button Pressed \n");
				AE->sfx("{ce969287-97e3-4324-b52b-f2f31edf0143}");
			}

			//Button held down
			if (state.Keyboard.GetKeyState(SDL_SCANCODE_A) == ButtonState::Held)
			{
				printf("A Button Held \n");
			}

		case SDL_KEYUP:
			//Button released after being pressed or held
			if (state.Keyboard.GetKeyState(SDL_SCANCODE_A) == ButtonState::Released)
			{
				printf("A Buton Released \n");
				AE->sfx("{cecb4df2-fbcf-4d3e-94ef-d261ec18747b}");
			}

			if (state.Keyboard.GetKeyState(SDL_SCANCODE_W) == ButtonState::Released)
			{
				//printf("W Buton Released \n");
			}
		}
	}

	updatingActors = true;
	for (auto actor : actors)
	{
		actor->ProcessInput(state);
	}
	updatingActors = false;
}

void Game::UpdateGame() {
	AE->update();
	//Compute delta time
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksCount + 16));

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
		pending->ComputeWorldTransform();
		actors.emplace_back(pending);
	}
	pendingActors.clear();

	//Add dead actors to a temp vector
	std::vector<Actor*> deadActors;
	for (auto actor : actors) {
		if (actor->GetState() == Actor::State::Dead) {
			deadActors.emplace_back(actor);
		}
	}

	//Delete dead actors
	for (auto actor : deadActors) {
		delete actor;
	}
}

void Game::GenerateOutput() {
	renderer->Draw();
}

void Game::LoadData() {
	Actor* a = new Actor(this);
	a->SetPosition(Eigen::Vector3f(200.0f, 75.0f, 0.0f));
	a->SetScale(100.0f);
	Eigen::Quaternionf q = Math::CreateQuaternionFromAngleAxis(Eigen::Vector3f::UnitZ(), -Math::Pi / 2.0f);
	q = Math::CreateQuaternionFromConcatenation(q,
		Math::CreateQuaternionFromAngleAxis(Eigen::Vector3f::UnitZ(), Math::Pi + Math::Pi / 4.0f));
	a->SetRotation(q);
	MeshComponent* mc = new MeshComponent(a);
	mc->SetMesh(renderer->GetMesh("cube.gpmesh"));

	a = new Actor(this);
	a->SetPosition(Eigen::Vector3f(200.0f, -75.0f, 0.0f));
	a->SetScale(3.0f);
	mc = new MeshComponent(a);
	//mc->SetMesh(renderer->GetMesh("Sphere.gpmesh"));

	const float start = -1250.0f;
	const float size = 250.0f;
	/*for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			a = new PlaneActor(this);
			a->SetPosition(Eigen::Vector3f(start + i * size, start + j * size, -100.0f));
		}
	}*/

	q = Math::CreateQuaternionFromAngleAxis(Eigen::Vector3f::UnitX(), Math::Pi / 2);

	/*for (int i = 0; i < 10; i++) {
		a = new PlaneActor(this);
		a->SetPosition(Vector3(start + i * size, start - size, 0.0f));
		a->SetRotation(q);

		a = new PlaneActor(this);
		a->SetPosition(Vector3(start + i * size, -start + size, 0.0f));
		a->SetRotation(q);
	}*/
	
	//Setup Lights
	renderer->SetAmbientLight(Eigen::Vector3f(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = renderer->GetDirectionalLight();
	dir.direction = Eigen::Vector3f(0.0f, 0.707f, -0.707f);
	dir.diffuseColour = Eigen::Vector3f(0.78f, 0.88f, 1.0f);
	dir.specColour = Eigen::Vector3f(0.8f, 0.8f, 0.8f);

	//Camera actor
	cameraActor = new CameraActor(this);

	//UI elements
	a = new Actor(this);
	a->SetPosition(Eigen::Vector3f(-350.0f, -350.0f, 0.0f));
	SpriteComponent* sc = new SpriteComponent(a);
	sc->SetTexture(renderer->GetTexture("hp.png"));

	a = new Actor(this);
	a->SetPosition(Eigen::Vector3f(-350.0f, -350.0f, 0.0f));
	a->SetScale(0.75f);
	sc = new SpriteComponent(a);
	sc->SetTexture(renderer->GetTexture("radar.png"));
}

void Game::UnloadData() {
	while (!actors.empty()) {
		delete actors.back();
	}
	if (renderer) {
		renderer->UnloadData();
	}
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

void Game::InitGUI()
{
	gui = new GUI(_argc, _argv);
	gui->createWindow();
}

void Game::Quit_GUI()
{
	delete gui;
}

void Game::Shutdown() {
	UnloadData();
	if (renderer) {
		renderer->Shutdown();
	}
	SDL_Quit();

	Quit_GUI();
}