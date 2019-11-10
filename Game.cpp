#include "Game.h"
#include <algorithm>
#include "Renderer.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "PlaneActor.h"
#include "CameraTargetActor.h"
#include "PointLightComponent.h"
#include "CubeActor.h"
#include "Generator.h"
#include "enemy.h"

Generator randGen;
Room* rooms;

Game::Game()
	:renderer(nullptr),
	isRunning(true),
	updatingActors(false),
	isReturning(false),
	scene(0)
{
	inputSystem = new InputSystem();
	AE = new AudioEngine();
	ticksCount = 0;

	playerCombat = new PlayerCombatSystem();
	playerLevels = new LevelUpSystem();
	enemyCombat = new EnemyCombatSystem(25, 10, 100);

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
		if (isLoading) {
			UnloadData();
			LoadData();
			isLoading = false;
		}
		else {
			ProcessInput();
			UpdateGame();
			GenerateOutput();
		}
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
				savedPlayerPosition = cameraTargetActor->GetPosition();
				isLoading = true;
				isReturning = false;
				scene = 1;
				//AE->sfx("{ce969287-97e3-4324-b52b-f2f31edf0143}");
			}


			if (state.Keyboard.GetKeyState(SDL_SCANCODE_E) == ButtonState::Pressed){
				printf("E Button Held \n");
				isLoading = true;
				isReturning = true;
				scene = 0;
				//AE->sfx("{cecb4df2-fbcf-4d3e-94ef-d261ec18747b}");
			}

			if (state.Keyboard.GetKeyState(SDL_SCANCODE_H) == ButtonState::Pressed) {
				printf("H Button Pressed \n");

				printf("\n\n");
				cout << "------PRE COMBAT ROUND------\n" << endl;

				cout << "***PLAYER STATS***" << endl;
				cout << "Player Base Health: " << playerCombat->getBaseHealth() << endl;
				cout << "Player Current Health: " << playerCombat->getCurrentHealth() << endl;
				cout << "Player Base Atk: " << playerCombat->getBaseAtk() << endl;
				cout << "Player Current Atk: " << playerCombat->getCurrentAtk() << endl;
				cout << "Player Debuff Amount: " << playerCombat->getDebuffAmt() << endl;
				printf("\n\n");

				cout << "***PLAYER LEVELS***" << endl;
				cout << "Player is Level: " << playerLevels->getCurrentLevel() << endl;
				cout << "Player has " << playerLevels->getCurrentXP() << " XP right now" << endl;
				cout << "Player needs " << playerLevels->getRequiredXP() << " XP to level up" << endl;
				printf("\n\n");

				cout << "***ENEMY STATS***" << endl;
				cout << "Enemy Health: " << enemyCombat->getHealth() << endl;
				cout << "Enemy Atk: " << enemyCombat->getAtk() << endl;
				printf("\n\n");

				//Combat stuff
				//rand atk just to randomly switch between light and heavy atks, but when it's implemented in game we'll let the player decide what they wanna do
				int randAtk;
				srand(time(NULL));
				randAtk = rand() % 2;
				int playerAtk = playerCombat->dealDmg(randAtk);
				cout << "Enemy took " << playerAtk << " damage" << endl;
				enemyCombat->takeDmg(playerAtk);
				int enemyAtk = enemyCombat->performAtk();
				playerCombat->takeDmg(enemyAtk);
				string playerStatus;
				string enemyStatus;
				if (playerCombat->checkIfDead()) {
					playerStatus = "dead";
				}
				else {
					playerStatus = "alive";
				}

				if (enemyCombat->checkIfDead()) {
					enemyStatus = "dead";
					int XPAmt = enemyCombat->getXP();
					bool doesLevel = playerLevels->addXP(XPAmt);
					if (doesLevel) {
						playerCombat->increaseStats();
					}
				}
				else {
					enemyStatus = "alive";
				}

				printf("\n\n");
				cout << "------POST COMBAT ROUND------\n" << endl;

				cout << "***PLAYER STATS***" << endl;
				cout << "Player Base Health: " << playerCombat->getBaseHealth() << endl;
				cout << "Player Current Health: " << playerCombat->getCurrentHealth() << endl;
				cout << "Player Base Atk: " << playerCombat->getBaseAtk() << endl;
				cout << "Player Current Atk: " << playerCombat->getCurrentAtk() << endl;
				cout << "Player Debuff Amount: " << playerCombat->getDebuffAmt() << endl;
				cout << "Player is " << playerStatus << endl;
				printf("\n\n");

				cout << "***PLAYER LEVELS***" << endl;
				cout << "Player is Level: " << playerLevels->getCurrentLevel() << endl;
				cout << "Player has " << playerLevels->getCurrentXP() << " XP right now" << endl;
				cout << "Player needs " << playerLevels->getRequiredXP() << " XP to level up" << endl;
				printf("\n\n");

				cout << "***ENEMY STATS***" << endl;
				cout << "Enemy Health: " << enemyCombat->getHealth() << endl;
				cout << "Enemy Atk: " << enemyCombat->getAtk() << endl;
				cout << "Enemy is " << enemyStatus << endl;
				printf("\n\n");
			}

		case SDL_KEYUP:
			//update game
			for (int r = 0; r < randGen.getNumRooms(); r++) {
				vector<enemy> useEnemy = randGen.getEnemies(r);
				for (int e = 0; e < useEnemy.size(); e++) {
					useEnemy.at(e).update();
				}
			}

			//check keyinput
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

			//for each enemy in the vector -> run the update function
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
	if (scene == 0) {
		if (isReturning) {
			int offsetX = 0;
			int offsetY = 0;

			int map_rows = 400;
			int map_cols = 400;

			map2D = new int* [map_rows];

			for (int i = 0; i < map_rows; i++) {
				map2D[i] = new int[map_cols + 1];
				map2D[i][0] = map_cols;

				for (int j = 1; j <= map_cols; j++)
					map2D[i][j] = NULL;
			}

			enemy setup;
			int count = 0;
			//enemies.assign(0, setup);

			//{ _width, _height, _entry, _entryDoor, _exit, _exitDoor, _isStart, _isEnd, _stairX, _stairY, _nextRoomCorridor };
			//for each room
			for (int r = 0; r < randGen.getNumRooms(); r++) {
				int* paramsTemp = rooms[r].getParameters();

				int width1 = randGen.getWidth(r);
				int height1 = randGen.getHeight(r);
				cout << "WIDTH: " << width1 << endl;
				cout << "HEIGHT: " << height1 << endl;
				cout << "OFFSETx: " << offsetX << endl;
				cout << "OFFSETy: " << offsetY << endl;
				cout << "CORRIDOR LENGTH: " << randGen.getCorridorLength(r) << endl;
				cout << "entryDoor: " << randGen.getEntryDoor(r) << endl;

				const float start = 0;
				const float size = 100.0f;
				int rows = 0;
				int cols = 0;
				int enemyX = 0;
				int enemyY = 0;
				//generates for each tile
				for (int i = 0; i < height1; i++) {
					for (int j = 0; j < width1; j++) {
						rows = offsetY + start + i;
						cols = offsetX + start + j;
						map2D[rows + 50][cols + 50] = 1;
						a = new PlaneActor(this);
						Vector3 pos = Vector3(rows * size, cols * size, -100.0f);
						a->SetPosition(pos);
						a->SetScale(1.f);
						CreatePointLights(a, pos, i + j);
					}
				}

				vector<enemy> useEnemy = randGen.getEnemies(r);
				for (int e = 0; e < useEnemy.size(); e++) {
					cubeActor = new CubeActor(this);

					int tempX = useEnemy.at(e).getPosition()[0];
					int tempY = useEnemy.at(e).getPosition()[1];

					enemyX = offsetX + start + tempX;
					enemyY = offsetY + start + tempY;

					cout << "ROOM:  " << r << " enemy: " << e << " At position: [" << tempX << ", " << tempY << "]" << endl;
					cout << "Expected out: [" << enemyX << ", " << enemyY << "]" << endl;

					map2D[enemyY + 50][enemyX + 50] = 2;
					Vector3 pos = Vector3(enemyY * size, enemyX * size, 0.0f);
					cubeActor->SetPosition(pos);
					cubeActor->SetScale(50.f);

					cout << " INSTANTIATING WITH : " << cubeActor;
					useEnemy.at(e).setup(cubeActor);
					cout << " SAVED:  " << useEnemy.at(e).getActor();
				}

				//enemies.insert(enemies.end(), useEnemy.begin(), useEnemy.end());

				//for corridor
				if (!randGen.getIsEnd(r)) {
					if ((bool)randGen.getExitLocation(r)) { // if north
						for (int z = 0; z < randGen.getCorridorLength(r); z++) {
							rows = offsetY + height1 + z;
							cols = offsetX + randGen.getExitDoor(r);
							map2D[rows + 50][cols + 50] = 1;
							a = new PlaneActor(this);

							Vector3 pos = Vector3(rows * size, cols * size, -100.0f);
							a->SetPosition(pos);
							a->SetScale(1.f);
							CreatePointLights(a, pos, z);

						}
						cout << "corridor placement: " << offsetX + randGen.getExitLocation(r) << endl;
						cout << " IS True " << endl;
						offsetY += height1 + randGen.getCorridorLength(r);
						offsetX += randGen.getExitDoor(r) - randGen.getEntryDoor(r + 1);


					}
					else { //if east
						for (int z = 0; z < randGen.getCorridorLength(r); z++) {
							rows = offsetY + randGen.getExitDoor(r);
							cols = offsetX + width1 + z;
							map2D[rows + 50][cols + 50] = 1;
							a = new PlaneActor(this);

							Vector3 pos = Vector3(rows * size, cols * size, -100.0f);
							a->SetPosition(pos);
							a->SetScale(1.f);
							CreatePointLights(a, pos, z);

						}
						cout << "corridor placement: " << offsetY + randGen.getExitLocation(r) << endl;
						cout << " IS FALSE " << endl;
						offsetX += width1 + randGen.getCorridorLength(r);
						offsetY += randGen.getExitDoor(r) - randGen.getEntryDoor(r + 1);
					}
				}
			}

			// Setup lights
			renderer->SetAmbientLight(Vector3(1.f, 1.f, 1.f));
			DirectionalLight& dir = renderer->GetDirectionalLight();
			dir.direction = Vector3(.0f, .0f, .0f);
			dir.diffuseColor = Vector3(0.78f, 0.88f, 1.0f);
			dir.specColor = Vector3(11.8f, 0.5f, 0.5f);


			// UI elements
			a = new Actor(this);
			a->SetPosition(Vector3(-350.0f, -350.0f, 0.0f));
			SpriteComponent* sc = new SpriteComponent(a);
			sc->SetTexture(renderer->GetTexture("Assets/cyan.png"));

			cameraTargetActor = new CameraTargetActor(this);
			cameraTargetActor->SetPosition(savedPlayerPosition);
			isReturning = false;
		} else { // ----------------------------------------------------------------------------------------------------------------------
			rooms = randGen.generate();

			int offsetX = 0;
			int offsetY = 0;

			int map_rows = 400;
			int map_cols = 400;

			map2D = new int* [map_rows];

			for (int i = 0; i < map_rows; i++) {
				map2D[i] = new int[map_cols + 1];
				map2D[i][0] = map_cols;

				for (int j = 1; j <= map_cols; j++)
					map2D[i][j] = NULL;
			}

			enemy setup;
			int count = 0;
			//enemies.assign(0, setup);

			//{ _width, _height, _entry, _entryDoor, _exit, _exitDoor, _isStart, _isEnd, _stairX, _stairY, _nextRoomCorridor };
			//for each room
			for (int r = 0; r < randGen.getNumRooms(); r++) {
				int* paramsTemp = rooms[r].getParameters();

				int width1 = randGen.getWidth(r);
				int height1 = randGen.getHeight(r);
				cout << "WIDTH: " << width1 << endl;
				cout << "HEIGHT: " << height1 << endl;
				cout << "OFFSETx: " << offsetX << endl;
				cout << "OFFSETy: " << offsetY << endl;
				cout << "CORRIDOR LENGTH: " << randGen.getCorridorLength(r) << endl;
				cout << "entryDoor: " << randGen.getEntryDoor(r) << endl;

				const float start = 0;
				const float size = 100.0f;
				int rows = 0;
				int cols = 0;
				int enemyX = 0;
				int enemyY = 0;
				//generates for each tile
				for (int i = 0; i < height1; i++) {
					for (int j = 0; j < width1; j++) {
						rows = offsetY + start + i;
						cols = offsetX + start + j;
						map2D[rows + 50][cols + 50] = 1;
						a = new PlaneActor(this);
						Vector3 pos = Vector3(rows * size, cols * size, -100.0f);
						a->SetPosition(pos);
						a->SetScale(1.f);
						CreatePointLights(a, pos, i + j);
					}
				}

				vector<enemy> useEnemy = randGen.getEnemies(r);
				for (int e = 0; e < useEnemy.size(); e++) {
					cubeActor = new CubeActor(this);

					int tempX = useEnemy.at(e).getPosition()[0];
					int tempY = useEnemy.at(e).getPosition()[1];

					enemyX = offsetX + start + tempX;
					enemyY = offsetY + start + tempY;

					cout << "ROOM:  " << r << " enemy: " << e << " At position: [" << tempX << ", " << tempY << "]" << endl;
					cout << "Expected out: [" << enemyX << ", " << enemyY << "]" << endl;

					map2D[enemyY + 50][enemyX + 50] = 2;
					Vector3 pos = Vector3(enemyY * size, enemyX * size, 0.0f);
					cubeActor->SetPosition(pos);
					cubeActor->SetScale(50.f);

					cout << " INSTANTIATING WITH : " << cubeActor;
					useEnemy.at(e).setup(cubeActor);
					cout << " SAVED:  " << useEnemy.at(e).getActor();
				}

				//enemies.insert(enemies.end(), useEnemy.begin(), useEnemy.end());

				//for corridor
				if (!randGen.getIsEnd(r)) {
					if ((bool)randGen.getExitLocation(r)) { // if north
						for (int z = 0; z < randGen.getCorridorLength(r); z++) {
							rows = offsetY + height1 + z;
							cols = offsetX + randGen.getExitDoor(r);
							map2D[rows + 50][cols + 50] = 1;
							a = new PlaneActor(this);

							Vector3 pos = Vector3(rows * size, cols * size, -100.0f);
							a->SetPosition(pos);
							a->SetScale(1.f);
							CreatePointLights(a, pos, z);

						}
						cout << "corridor placement: " << offsetX + randGen.getExitLocation(r) << endl;
						cout << " IS True " << endl;
						offsetY += height1 + randGen.getCorridorLength(r);
						offsetX += randGen.getExitDoor(r) - randGen.getEntryDoor(r + 1);


					}
					else { //if east
						for (int z = 0; z < randGen.getCorridorLength(r); z++) {
							rows = offsetY + randGen.getExitDoor(r);
							cols = offsetX + width1 + z;
							map2D[rows + 50][cols + 50] = 1;
							a = new PlaneActor(this);

							Vector3 pos = Vector3(rows * size, cols * size, -100.0f);
							a->SetPosition(pos);
							a->SetScale(1.f);
							CreatePointLights(a, pos, z);

						}
						cout << "corridor placement: " << offsetY + randGen.getExitLocation(r) << endl;
						cout << " IS FALSE " << endl;
						offsetX += width1 + randGen.getCorridorLength(r);
						offsetY += randGen.getExitDoor(r) - randGen.getEntryDoor(r + 1);
					}
				}
			}
			//cout << " LAST ENEMY ACTOR: " << randGen.getEnemies(9).at(0).getActor() << endl;

			// Setup lights
			renderer->SetAmbientLight(Vector3(1.f, 1.f, 1.f));
			DirectionalLight& dir = renderer->GetDirectionalLight();
			dir.direction = Vector3(.0f, .0f, .0f);
			dir.diffuseColor = Vector3(0.78f, 0.88f, 1.0f);
			dir.specColor = Vector3(11.8f, 0.5f, 0.5f);


			// UI elements
			a = new Actor(this);
			a->SetPosition(Vector3(-350.0f, -350.0f, 0.0f));
			SpriteComponent* sc = new SpriteComponent(a);
			sc->SetTexture(renderer->GetTexture("Assets/cyan.png"));

			cameraTargetActor = new CameraTargetActor(this);
		}
	}
	else if (scene == 1) {

	}
}

int Game::IsWalkable(int row, int col) {
	int walkable = 0;
	if (map2D[row + 50][col + 50] == 1) {
		walkable = 1;
	}
	else if (map2D[row + 50][col + 50] == 2){
		walkable = 2;
	}
	return walkable;
}

void Game::CreatePointLights(Actor*& a, Vector3& pos, int z)
{
	// Create some point lights
	a = new Actor(this);
	pos.z += 50.f;
	a->SetPosition(pos);
	PointLightComponent* p = new PointLightComponent(a);
	Vector3 color;
	switch (z % 5)
	{
	case 0:
		color = Color::Green;
		break;
	case 1:
		color = Color::Blue;
		break;
	case 2:
		color = Color::Red;
		break;
	case 3:
		color = Color::Yellow;
		break;
	case 4:
		color = Color::LightPink;
		break;
	}
	p->diffuseColor = color;
	p->innerRadius = 50.0f;
	p->outerRadius = 100.0f;
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