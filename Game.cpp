#include "Game.h"
#include <algorithm>
#include "Renderer.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "PlaneActor.h"
#include "CameraTargetActor.h"
#include "PointLightComponent.h"

Game::Game()
	:renderer(nullptr),
	isRunning(true),
	updatingActors(false)
{
	inputSystem = new InputSystem();
	AE = new AudioEngine();
	ticksCount = 0;
}

bool Game::Initialize(){
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0){
		SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
		return false;
	}

	if (!inputSystem->Initialize()){
		SDL_Log("Failed to initialize input system");
		return false;
	}

	// Create the renderer
	renderer = new Renderer(this);
	if (!renderer->Initialize(1024.0f, 768.0f)){
		SDL_Log("Failed to initialize renderer");
		delete renderer;
		renderer = nullptr;
		return false;
	}

	AE->setup();
	//AE->sfx("{8a6a04bd-f459-4efe-9b9f-5b2bd9969d8c}");
	LoadData();
	ticksCount = SDL_GetTicks();
	printf("%s%lu\n", "Number of Ticks Initialized: ", ticksCount);
	return true;
}

void Game::RunLoop(){
	while (isRunning){
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput() {
	SDL_Event event;
	inputSystem->BeforeUpdate();
	const InputState& state = inputSystem->GetState();

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			/*case SDL_QUIT:
				isRunning = false;
				break;*/

		case SDL_KEYDOWN:
			//Quit Game
			if (state.Keyboard.GetKeyState(SDL_SCANCODE_ESCAPE) == ButtonState::Pressed){
				isRunning = false;
			}

			if (state.Keyboard.GetKeyState(SDL_SCANCODE_W) == ButtonState::Held){
				printf("W Button Held \n");
			}

			if (state.Keyboard.GetKeyState(SDL_SCANCODE_A) == ButtonState::Held){
				printf("A Button Held \n");
			}

			if (state.Keyboard.GetKeyState(SDL_SCANCODE_S) == ButtonState::Held){
				printf("S Button Held \n");
			}

			if (state.Keyboard.GetKeyState(SDL_SCANCODE_D) == ButtonState::Held){
				printf("D Button Held \n");
			}

			if (state.Keyboard.GetKeyState(SDL_SCANCODE_Q) == ButtonState::Pressed){
				printf("Q Button Held \n");
				AE->sfx("{ce969287-97e3-4324-b52b-f2f31edf0143}");
			}


			if (state.Keyboard.GetKeyState(SDL_SCANCODE_E) == ButtonState::Pressed){
				printf("E Button Held \n");
				AE->sfx("{cecb4df2-fbcf-4d3e-94ef-d261ec18747b}");
			}

		case SDL_KEYUP:
			if (state.Keyboard.GetKeyState(SDL_SCANCODE_A) == ButtonState::Released){
				printf("A Button Released \n");
			}

			if (state.Keyboard.GetKeyState(SDL_SCANCODE_W) == ButtonState::Released){
				printf("W Button Released \n");
			}

			if (state.Keyboard.GetKeyState(SDL_SCANCODE_S) == ButtonState::Released){
				printf("S Button Released \n");
			}

			if (state.Keyboard.GetKeyState(SDL_SCANCODE_D) == ButtonState::Released){
				printf("D Button Released \n");
			}

			if (state.Keyboard.GetKeyState(SDL_SCANCODE_Q) == ButtonState::Released){
				printf("Q Button Released \n");
			}

			if (state.Keyboard.GetKeyState(SDL_SCANCODE_E) == ButtonState::Released){
				printf("E Button Released \n");
			}
		}
	}

	updatingActors = true;
	for (auto actor : actors){
		actor->ProcessInput(state);
	}
	updatingActors = false;
}

void Game::UpdateGame()
{
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksCount + 16));

	float deltaTime = (SDL_GetTicks() - ticksCount) / 1000.0f;
	if (deltaTime > 0.05f){
		deltaTime = 0.05f;
	}
	ticksCount = SDL_GetTicks();

	// Update all actors
	updatingActors = true;
	for (auto actor : actors){
		actor->Update(deltaTime);
	}
	updatingActors = false;

	for (auto pending : pendingActors){
		pending->ComputeWorldTransform();
		actors.emplace_back(pending);
	}
	pendingActors.clear();

	std::vector<Actor*> deadActors;
	for (auto actor : actors){
		if (actor->GetState() == Actor::State::Dead){
			deadActors.emplace_back(actor);
		}
	}

	for (auto actor : deadActors){
		delete actor;
	}

	AE->update();
}

void Game::GenerateOutput(){
	renderer->Draw();
}

void Game::LoadData(){
	
	Actor* a = new Actor(this);
	a->SetPosition(Vector3(200.0f, 75.0f, 0.0f));
	a->SetScale(100.0f);
	Quaternion q(Vector3::UnitY, -Math::PiOver2);
	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::Pi + Math::Pi / 4.0f));
	a->SetRotation(q);
	MeshComponent* mc = new MeshComponent(a);
	mc->SetMesh(renderer->GetMesh("Assets/Cube.obj"));

	// Setup floor
	const float start = -500.0f;
	const float size = 100.0f;
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 10; j++){
			a = new PlaneActor(this);
			a->SetPosition(Vector3(start + i * size, start + j * size, -100.0f));
			a->SetScale(1.f);
		}
	}

	// Setup lights
	renderer->SetAmbientLight(Vector3(1.f, 1.f, 1.f));
	DirectionalLight& dir = renderer->GetDirectionalLight();
	dir.direction = Vector3(1., -0.707f, -0.707f);
	dir.diffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.specColor = Vector3(0.8f, 0.8f, 0.8f);

	// UI elements
	//a = new Actor(this);
	//a->SetPosition(Vector3(-350.0f, -350.0f, 0.0f));
	//SpriteComponent* sc = new SpriteComponent(a);
	//sc->SetTexture(renderer->GetTexture("Assets/cyan.png"));

	cameraTargetActor = new CameraTargetActor(this);
}

void Game::UnloadData(){
	while (!actors.empty()){
		delete actors.back();
	}

	if (renderer){
		renderer->UnloadData();
	}
}

void Game::Shutdown(){
	UnloadData();
	if (renderer){
		renderer->Shutdown();
	}
	SDL_Quit();
}

void Game::AddActor(Actor* actor){
	if (updatingActors){
		pendingActors.emplace_back(actor);
	}else{
		actors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor){
	auto iter = std::find(pendingActors.begin(), pendingActors.end(), actor);
	if (iter != pendingActors.end()){
		std::iter_swap(iter, pendingActors.end() - 1);
		pendingActors.pop_back();
	}

	iter = std::find(actors.begin(), actors.end(), actor);
	if (iter != actors.end()){
		std::iter_swap(iter, actors.end() - 1);
		actors.pop_back();
	}
}