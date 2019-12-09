#include "Game.h"
#include <algorithm>
#include "Renderer.h"
#include "Room.h"
#include "Actor.h"
#include "EnemyActor.h"
#include "Generator.h"
#include "BossSprite.h"
#include "SkeletonSprite.h"
#include "VictorySprite.h"
#include "KnightSprite.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "PlaneActor.h"
#include "CameraTargetActor.h"
#include "PointLightComponent.h"
#include "Skeleton.h"
#include "Animation.h"
#include "CubeActor.h"
#include "StairActor.h"
#include "HudElement.h"
#include "HUD.h"
#include "TextBox.h"

Game::Game()
	:renderer(nullptr),
	isRunning(true),
	updatingActors(false),
	isReturning(false),
	isAttacking(false),
	waitForEnemyAttack(false),
	enemyCollision(false),
	stairCollision(false),
	level(0),
	scene(0),
	doesWin(true)
{
	inputSystem = new InputSystem();
	AE = new AudioEngine();
	ticksCount = 0;
	currentAudioInstance = nullptr;
	randGen = new Generator(this);
	playerCombat = new PlayerCombatSystem();
	playerLevels = new LevelUpSystem();
	enemyCombat = new EnemyCombatSystem(50, 10, 100);
}

bool Game::Initialize() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
		return false;
	}

	if (!inputSystem->Initialize()) {
		SDL_Log("Failed to initialize input system");
		return false;
	}

	// Create the renderer
	renderer = new Renderer(this);
	if (!renderer->Initialize(1024.0f, 768.0f)) {
		SDL_Log("Failed to initialize renderer");
		delete renderer;
		renderer = nullptr;
		return false;
	}

	// Initialize SDL_ttf
	if (TTF_Init() != 0)
	{
		SDL_Log("Failed to initialize SDL_ttf");
		return false;
	}

	//InitFontRenderer();

	AE->setup();
	LoadData();
	ticksCount = SDL_GetTicks();
	printf("%s%lu\n", "Number of Ticks Initialized: ", ticksCount);
	return true;
}

void Game::RunLoop() {
	while (isRunning) {
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
			if (state.Keyboard.GetKeyState(SDL_SCANCODE_ESCAPE) == ButtonState::Pressed) {
				isRunning = false;
			}

			if (state.Keyboard.GetKeyState(SDL_SCANCODE_H) == ButtonState::Pressed) {
				printf("H Button Pressed \n");
				if ((scene == ENEMY_FIGHT_SCENE || scene == BOSS_FIGHT_SCENE) && !isAttacking) {
					thread th1(&AudioEngine::playerHeavyAtk, AE);
					th1.join();
					CombatRound(1);
				}
			}

			if (state.Keyboard.GetKeyState(SDL_SCANCODE_L) == ButtonState::Pressed) {
				printf("L Button Pressed \n");
				if ((scene == ENEMY_FIGHT_SCENE || scene == BOSS_FIGHT_SCENE) && !isAttacking) {
					thread th2(&AudioEngine::playerAtk, AE);
					th2.join();
					CombatRound(0);
				}
			}

			if (state.Keyboard.GetKeyState(SDL_SCANCODE_R) == ButtonState::Pressed) {
				printf("R Button Pressed \n");
				if (scene == END_GAME_SCENE) {
					scene = MAZE_SCENE;
					level = 0;
					enemyCombat->setBaseHealth(50);
					enemyCombat->setAtk(10);
					enemyCombat->setXP(50);
					enemyCombat->resetEnemy();
					playerCombat->setBaseHealth(100);
					playerCombat->setBaseAtk(25);
					playerCombat->setCurrentHealth(playerCombat->getBaseHealth());
					playerCombat->setCurrentAtk(playerCombat->getBaseAtk());
					playerCombat->setDebuffAmt(0);
					playerLevels->setCurrentLevel(1);
					playerLevels->setCurrentXP(0);
					playerLevels->setRequiredXP(100);
					doesWin = true;
					isReturning = false;
					isLoading = true;
				}
			}
		}
	}

	updatingActors = true;
	for (auto actor : actors) {
		actor->ProcessInput(state);
	}
	updatingActors = false;
}

void Game::UpdateGame()
{
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksCount + 16));

	float deltaTime = (SDL_GetTicks() - ticksCount) / 1000.0f;
	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}
	ticksCount = SDL_GetTicks();

	// Update all actors
	updatingActors = true;
	for (auto actor : actors) {
		if (!enemyCollision)
			actor->Update(deltaTime);
	}
	updatingActors = false;

	for (auto pending : pendingActors) {
		pending->ComputeWorldTransform();
		actors.emplace_back(pending);
	}
	pendingActors.clear();

	std::vector<Actor*> deadActors;
	for (auto actor : actors) {
		if (actor->GetState() == Actor::State::Dead) {
			deadActors.emplace_back(actor);
		}
	}

	for (auto actor : deadActors) {
		delete actor;
	}

	AE->update();

	if (enemyCollision) {
		
		gameMessage_text->UpdateText("entering combat mode");
		
		savedPlayerPosition = cameraTargetActor->GetPosition();
		for (Actor* enemyPos : enems) {
			saved_enemies.push_back(enemyPos->GetPosition());
		}
		scene = ENEMY_FIGHT_SCENE;
		AE->stopAudio(currentAudioInstance);
		currentAudioInstance = AE->startFightBGM();
		isLoading = true;
		enemyCollision = false;
	}
	else if (stairCollision) {
		
		gameMessage_text->UpdateText("Stairs found!");
		
		enemyCombat->enemyLevel(10, 10, 50);
		if (level >= 5) {
			if (currentAudioInstance) {
				AE->stopAudio(currentAudioInstance);
			}
			currentAudioInstance = AE->startBossBGM();
			scene = BOSS_FIGHT_SCENE;
			enemyCombat->enemyLevel(150, 30, 500);
		}
		isLoading = true;
		stairCollision = false;
	}
	else if (isAttacking && skeletonSprite->ready) {
		if (scene == ENEMY_FIGHT_SCENE && knightSprite->ready) {
			if (waitForEnemyAttack) {
				knightSprite->SwitchState(KnightSprite::Attacking);
				waitForEnemyAttack = false;
			}
			else {
				isAttacking = false;
				
				playerHealth_text->UpdateText("player health: " + std::to_string(playerCombat->getCurrentHealth()));
				enemyHealth_text->UpdateText("enemy health: " + std::to_string(enemyCombat->getCurrentHealth()));
				
			}
		}
		else if (scene == BOSS_FIGHT_SCENE && bossSprite->ready) {
			if (waitForEnemyAttack) {
				bossSprite->SwitchState(BossSprite::Attacking);
				waitForEnemyAttack = false;
			}
			else {
				isAttacking = false;
				
				playerHealth_text->UpdateText("player health: " + std::to_string(playerCombat->getCurrentHealth()));
				enemyHealth_text->UpdateText("enemy health: " + std::to_string(enemyCombat->getCurrentHealth()));
				
			}
		}
	}

	if (scene == ENEMY_FIGHT_SCENE) {
		if (enemyCombat->getCurrentHealth() <= 0) {
			playerCombat->setDebuffAmt(0);
			thread th3(&AudioEngine::enemyDeath, AE);
			th3.join();
			_asm {
				//resets the enemy health back after killing an enemy
				mov		eax, dword ptr[this] //copy all variables into eax register
				mov		ecx, dword ptr[eax]Game.enemyCombat //from eax register copy only enemyCombat address into ecx register
				call	EnemyCombatSystem::resetEnemy //call the reset enemy function for enemy combat

				//sets isLoading bool to true
				mov		eax, dword ptr[this] //copy all variables into eax register
				mov[eax]Game.isLoading, 1 //from eax register access and change isLoading boolean to true

				//sets isReturning bool to true
				mov		eax, dword ptr[this] //copy all variables into eax register
				mov[eax]Game.isReturning, 1 //from eax register access and change isReturning boolean to true

				//sets scene value to 0
				mov		eax, dword ptr[this] //copy all variables into eax register
				mov[eax]Game.scene, 0 //from eax register access and change scene int to 0
			}
		}

		if (playerCombat->getCurrentHealth() <= 0) {
			doesWin = false;
			isLoading = true;
			scene = END_GAME_SCENE;
		}
	}
	else if (scene == BOSS_FIGHT_SCENE) {
		if (enemyCombat->getCurrentHealth() <= 0) {
			doesWin = true;
			isLoading = true;
			scene = END_GAME_SCENE;
		}
		else if (playerCombat->getCurrentHealth() <= 0) {
			doesWin = false;
			isLoading = true;
			scene = END_GAME_SCENE;
		}
	}
}

void Game::GenerateOutput() {
	renderer->Draw();
}

void Game::LoadData() {
	Actor* a = new Actor(this);
	InitHUD();
	if (scene == MAZE_SCENE) {
		if (currentAudioInstance) {
			AE->stopAudio(currentAudioInstance);
		}
		currentAudioInstance = AE->startDungeonBGM();
		if (isReturning) {
			enems.clear();

			int offsetX = 0;
			int offsetY = 0;

			int map_rows = 400;
			int map_cols = 400;

			map2D = new int* [map_rows];

			const float size = 100.0f;

			for (int i = 0; i < map_rows; i++) {
				map2D[i] = new int[map_cols + 1];
				map2D[i][0] = map_cols;

				for (int j = 1; j <= map_cols; j++)
					map2D[i][j] = NULL;
			}

			cameraTargetActor = new CameraTargetActor(this);
			cameraTargetActor->SetPosition(savedPlayerPosition);

			//{ _width, _height, _entry, _entryDoor, _exit, _exitDoor, _isStart, _isEnd, _stairX, _stairY, _nextRoomCorridor };
			//for each room
			for (int r = 0; r < randGen->getNumRooms(); r++) {
				int* paramsTemp = rooms[r]->getParameters();

				int width1 = randGen->getWidth(r);
				int height1 = randGen->getHeight(r);
				/*cout << "WIDTH: " << width1 << endl;
				cout << "HEIGHT: " << height1 << endl;
				cout << "OFFSETx: " << offsetX << endl;
				cout << "OFFSETy: " << offsetY << endl;
				cout << "CORRIDOR LENGTH: " << randGen->getCorridorLength(r) << endl;
				cout << "entryDoor: " << randGen->getEntryDoor(r) << endl;*/

				const float start = 0;
				int rows = 0;
				int cols = 0;
				int enemyX = 0;
				int enemyY = 0;
				//generatzzes for each tile
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

				for (Vector3 savedEnemy : saved_enemies) { // *************************************************************************************
					if (!(savedEnemy.x == savedPlayerPosition.x && savedEnemy.y == savedPlayerPosition.y)) {
						enemyActor = new EnemyActor(this);
						enems.push_back(enemyActor);
						enemyActor->SetPosition(savedEnemy);
						enemyActor->SetSkeletalMesh();
						enemyActor->SetMoveable(true);

						rows = (int)(savedEnemy.x / 100);
						cols = (int)(savedEnemy.y / 100);
						//SetEnemyMapPos(rows-50, cols-50);
					}
				}
				saved_enemies.clear();

				//enemies.insert(enemies.end(), useEnemy.begin(), useEnemy.end());

				//for corridor
				if (!randGen->getIsEnd(r)) {
					if ((bool)randGen->getExitLocation(r)) { // if north
						for (int z = 0; z < randGen->getCorridorLength(r); z++) {
							rows = offsetY + height1 + z;
							cols = offsetX + randGen->getExitDoor(r);
							map2D[rows + 50][cols + 50] = 1;
							a = new PlaneActor(this);

							Vector3 pos = Vector3(rows * size, cols * size, -100.0f);
							a->SetPosition(pos);
							a->SetScale(1.f);
							CreatePointLights(a, pos, z);

						}
						/*cout << "corridor placement: " << offsetX + randGen->getExitLocation(r) << endl;
						cout << " IS True " << endl;*/
						offsetY += height1 + randGen->getCorridorLength(r);
						offsetX += randGen->getExitDoor(r) - randGen->getEntryDoor(r + 1);


					}
					else { //if east
						for (int z = 0; z < randGen->getCorridorLength(r); z++) {
							rows = offsetY + randGen->getExitDoor(r);
							cols = offsetX + width1 + z;
							map2D[rows + 50][cols + 50] = 1;
							a = new PlaneActor(this);

							Vector3 pos = Vector3(rows * size, cols * size, -100.0f);
							a->SetPosition(pos);
							a->SetScale(1.f);
							CreatePointLights(a, pos, z);

						}
						/*cout << "corridor placement: " << offsetY + randGen->getExitLocation(r) << endl;
						cout << " IS FALSE " << endl;*/
						offsetX += width1 + randGen->getCorridorLength(r);
						offsetY += randGen->getExitDoor(r) - randGen->getEntryDoor(r + 1);
					}
				}
			}

			a = new StairActor(this);
			int tempX = offsetX + randGen->getStairX();
			int tempY = offsetY + randGen->getStairY();

			map2D[tempY + 50][tempX + 50] = 4;

			//cout << tempX << " :TEMPX" << endl;
			//cout << tempY << " :TEMPY" << endl;

			a->SetPosition(Vector3(tempY * size, tempX * size, -50.0f));
			a->SetScale(100.f);

			// Setup lights
			renderer->SetAmbientLight(Vector3(1.f, 1.f, 1.f));
			DirectionalLight& dir = renderer->GetDirectionalLight();
			dir.direction = Vector3(.0f, .0f, .0f);
			dir.diffuseColor = Vector3(0.78f, 0.88f, 1.0f);
			dir.specColor = Vector3(11.8f, 0.5f, 0.5f);

			// UI elements
			
			gameMessage_text->UpdateText("Find an exit point");
			
			isReturning = false;
		}
		else { // ----------------------------------------------------------------------------------------------------------------------

			cameraTargetActor = new CameraTargetActor(this);
			rooms = randGen->generate();

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


			int count = 0;
			const float size = 100.0f;
			//enemies.assign(0, setup);

			//{ _width, _height, _entry, _entryDoor, _exit, _exitDoor, _isStart, _isEnd, _stairX, _stairY, _nextRoomCorridor };
			//for each room
			for (int r = 0; r < randGen->getNumRooms(); r++) {
				int* paramsTemp = rooms[r]->getParameters();

				int width1 = randGen->getWidth(r);
				int height1 = randGen->getHeight(r);
				/*cout << "WIDTH: " << width1 << endl;
				cout << "HEIGHT: " << height1 << endl;
				cout << "OFFSETx: " << offsetX << endl;
				cout << "OFFSETy: " << offsetY << endl;
				cout << "CORRIDOR LENGTH: " << randGen->getCorridorLength(r) << endl;
				cout << "entryDoor: " << randGen->getEntryDoor(r) << endl;*/

				const float start = 0;
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

				vector<EnemyActor*> useEnemy = randGen->getEnemies(r); // **************************************************************************
				numEnemies.push_back(useEnemy.size());
				for (int e = 0; e < useEnemy.size(); e++) {
					enemyActor = new EnemyActor(this);
					enems.push_back(enemyActor);
					enemyActor->SetMoveable(true);

					int tempX = useEnemy[e]->GetPosition().x;
					int tempY = useEnemy[e]->GetPosition().y;

					enemyX = offsetX + start + tempX;
					enemyY = offsetY + start + tempY;

					map2D[enemyY + 50][enemyX + 50] = 2;
					Vector3 pos = Vector3(enemyY * size, enemyX * size, -100.0f);
					enemyActor->SetPosition(pos);
					enemyActor->SetMoveable(true);
					enemyActor->SetSkeletalMesh();
				}

				//for corridor
				if (!randGen->getIsEnd(r)) {
					if ((bool)randGen->getExitLocation(r)) { // if north
						for (int z = 0; z < randGen->getCorridorLength(r); z++) {
							rows = offsetY + height1 + z;
							cols = offsetX + randGen->getExitDoor(r);
							map2D[rows + 50][cols + 50] = 1;
							a = new PlaneActor(this);

							Vector3 pos = Vector3(rows * size, cols * size, -100.0f);
							a->SetPosition(pos);
							a->SetScale(1.f);
							CreatePointLights(a, pos, z);

						}
						/*cout << "corridor placement: " << offsetX + randGen->getExitLocation(r) << endl;
						cout << " IS True " << endl;*/
						offsetY += height1 + randGen->getCorridorLength(r);
						offsetX += randGen->getExitDoor(r) - randGen->getEntryDoor(r + 1);


					}
					else { //if east
						for (int z = 0; z < randGen->getCorridorLength(r); z++) {
							rows = offsetY + randGen->getExitDoor(r);
							cols = offsetX + width1 + z;
							map2D[rows + 50][cols + 50] = 1;
							a = new PlaneActor(this);

							Vector3 pos = Vector3(rows * size, cols * size, -100.0f);
							a->SetPosition(pos);
							a->SetScale(1.f);
							CreatePointLights(a, pos, z);

						}
						/*cout << "corridor placement: " << offsetY + randGen->getExitLocation(r) << endl;
						cout << " IS FALSE " << endl;*/
						offsetX += width1 + randGen->getCorridorLength(r);
						offsetY += randGen->getExitDoor(r) - randGen->getEntryDoor(r + 1);
					}
				}
			}

			a = new StairActor(this);
			int tempX = offsetX + randGen->getStairX();
			int tempY = offsetY + randGen->getStairY();

			map2D[tempY + 50][tempX + 50] = 4;

			/*cout << tempX + 50 << " :TEMPX" << endl;
			cout << tempY + 50 << " :TEMPY" << endl;*/

			a->SetPosition(Vector3(tempY * size, tempX * size, -10.0f));
			a->SetScale(100.f);

			//cout << " LAST ENEMY ACTOR: " << randGen.getEnemies(9).at(0).getActor() << endl;

			// Setup lights
			renderer->SetAmbientLight(Vector3(1.f, 1.f, 1.f));
			DirectionalLight& dir = renderer->GetDirectionalLight();
			dir.direction = Vector3(.0f, .0f, .0f);
			dir.diffuseColor = Vector3(0.78f, 0.88f, 1.0f);
			dir.specColor = Vector3(11.8f, 0.5f, 0.5f);

			// UI elements
			
			gameMessage_text->UpdateText("Find an exit point");
			
			level++;
		}

	}
	else if (scene == ENEMY_FIGHT_SCENE) {
		waitForEnemyAttack = false;
		AE->stopAudio(currentAudioInstance);
		currentAudioInstance = AE->startFightBGM();
		
		Actor* combatText = new Actor(this);
		combatText->SetPosition(Vector3(0.0f, -210.0f, 0.0f));
		SpriteComponent* sc = new SpriteComponent(combatText);
		sc->SetTexture(renderer->GetTexture("Assets/combatText.png"));
		
		// we wanna show it here as well
		
		playerHealth_text->UpdateText("player health: " + std::to_string(playerCombat->getCurrentHealth()));
		enemyHealth_text->UpdateText("enemy health: " + std::to_string(enemyCombat->getCurrentHealth()));
		
		skeletonSprite = new SkeletonSprite(this);
		knightSprite = new KnightSprite(this);
	}
	else if (scene == BOSS_FIGHT_SCENE) {
		waitForEnemyAttack = false;
		AE->stopAudio(currentAudioInstance);
		currentAudioInstance = AE->startBossBGM();
		enemyCombat = new EnemyCombatSystem(200, 25, 500);

		Actor* combatText = new Actor(this);
		combatText->SetPosition(Vector3(0.0f, -210.0f, 0.0f));
		SpriteComponent* sc = new SpriteComponent(combatText);
		sc->SetTexture(renderer->GetTexture("Assets/combatText.png"));

		// we wanna show it here as well
		
		playerHealth_text->UpdateText("player health: " + std::to_string(playerCombat->getCurrentHealth()));
		enemyHealth_text->UpdateText("enemy health: " + std::to_string(enemyCombat->getCurrentHealth()));
		
		skeletonSprite = new SkeletonSprite(this);

		bossSprite = new BossSprite(this);
		bossSprite->SetPosition(Vector3(350.0f, 50.0, 0.0f));
		bossSprite->SetScale(5.f);
	}
	else if (scene == END_GAME_SCENE) {
		string endTextStr;
		if (doesWin) {
			if (currentAudioInstance) {
				AE->stopAudio(currentAudioInstance);
			}
			currentAudioInstance = AE->startWinAltBGM();

			//victorySprite = new VictorySprite(this);
			endTextStr = "YOU WIN";
		}
		else {
			if (currentAudioInstance) {
				AE->stopAudio(currentAudioInstance);
			}
			currentAudioInstance = AE->startLoseBGM();
			endTextStr = "GAME OVER";
		}
		
		endMessage_text->UpdateText(endTextStr);
		replayMessage_text->UpdateText("PRESS R TO REPLAY");
		
	}
}

int Game::IsWalkable(int row, int col) {
	int walkable = 0;
	if (map2D[row + 50][col + 50] == 1) {
		walkable = 1;
	}
	else if (map2D[row + 50][col + 50] == 2) {
		walkable = 2;
	}
	else if (map2D[row + 50][col + 50] == 3) {
		walkable = 3;
	}
	else if (map2D[row + 50][col + 50] == 4) {
		walkable = 4;
	}
	return walkable;
}

void Game::SetWalkable(int row, int col) {
	map2D[row + 50][col + 50] = 1;
}

void Game::SetEnemyMapPos(int row, int col) {
	map2D[row + 50][col + 50] = 2;
}

void Game::SetPlayerMapPos(int row, int col) {
	map2D[row + 50][col + 50] = 3;
}

void Game::SetStairMapPos(int row, int col) {
	map2D[row + 50][col + 50] = 4;
}

void Game::CreatePointLights(Actor*& a, Vector3& pos, int z)
{
	// Create some point lights
	a = new Actor(this);
	pos.z += 50.f;
	a->SetPosition(pos);
	PointLightComponent* p = new PointLightComponent(a);
	Vector3 color;
	switch (rand() % 4)
	{
	case 0:
		color = Vector3(.4f, .2f, 0.f);
		break;
	case 1:
		color = Vector3(.3f, .1f, 0.f);
		break;
	case 2:
		color = Vector3(.5f, .2f, .1f);
		break;
	case 3:
		color = Vector3(.75f, .5f, .4f);
		break;
	}
	p->diffuseColor = color;
	p->innerRadius = 25.0f;
	p->outerRadius = 100.0f;
}

void Game::UnloadData() {


	UnloadHud();

	if (renderer) {
		renderer->UnloadData();
	}

	while (!actors.empty()) {
		delete actors.back();
	}
	actors.clear();
}

void Game::UnloadSkelAnim() {
	// Unload skeletons
	for (auto s : skeletons)
	{
		delete s.second;
	}

	// Unload animations
	for (auto a : anims)
	{
		delete a.second;
	}
}

void Game::Shutdown() {
	UnloadData();

	if (hud)
	{
		delete hud;
	}

	UnloadSkelAnim();

	if (renderer) {
		renderer->Shutdown();
	}
	SDL_Quit();
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

Skeleton* Game::GetSkeleton(const std::string& fileName)
{
	auto iter = skeletons.find(fileName);
	if (iter != skeletons.end())
	{
		return iter->second;
	}
	else
	{
		Skeleton* sk = new Skeleton();
		if (sk->Load(fileName))
		{
			skeletons.emplace(fileName, sk);
		}
		else
		{
			delete sk;
			sk = nullptr;
		}
		return sk;
	}
}

Animation* Game::GetAnimation(const std::string& fileName)
{
	auto iter = anims.find(fileName);
	if (iter != anims.end())
	{
		return iter->second;
	}
	else
	{
		Animation* anim = new Animation();
		if (anim->Load(fileName))
		{
			anims.emplace(fileName, anim);
		}
		else
		{
			delete anim;
			anim = nullptr;
		}
		return anim;
	}
}

//Atk type 0 = Light Atk
//Atk type 1 = Heavy Atk, 
void Game::CombatRound(int atkType) {
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
	cout << "Enemy Health: " << enemyCombat->getCurrentHealth() << endl;
	cout << "Enemy Atk: " << enemyCombat->getAtk() << endl;
	printf("\n\n");
	skeletonSprite->SwitchState(SkeletonSprite::Attacking);
	int playerAtk = playerCombat->dealDmg(atkType);
	if (playerAtk == 0)
	{
		if (scene == ENEMY_FIGHT_SCENE)
		{
			knightSprite->SwitchState(KnightSprite::Dodging);
		}
		else if (scene == BOSS_FIGHT_SCENE)
		{
			bossSprite->SwitchState(BossSprite::Dodging);
		}
	}
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

	printf("\n");
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
	cout << "Enemy Health: " << enemyCombat->getCurrentHealth() << endl;
	cout << "Enemy Atk: " << enemyCombat->getAtk() << endl;
	cout << "Enemy is " << enemyStatus << endl;

	isAttacking = true;
	waitForEnemyAttack = true;
	cout << "SCENE NUM: " << scene << endl;
}

void Game::InitHUD()
{
	hud = new HUD();
	
	playerHealth_text = (TextBox*)hud->addElement(new Actor(this), HUD::TEXT_BOX);
	//assert(playerHealth_text != NULL);
	playerHealth_text->SetPosition(Vector3(-300.0f, 180.0f, 0.0f));

	enemyHealth_text = (TextBox*)hud->addElement(new Actor(this), HUD::TEXT_BOX);
	enemyHealth_text->SetPosition(Vector3(300.0f, 180.0f, 0.0f));

	gameMessage_text = (TextBox*)hud->addElement(new Actor(this), HUD::TEXT_BOX);
	gameMessage_text->SetPosition(Vector3(-350.0f, -350.0f, 0.0f));

	endMessage_text = (TextBox*)hud->addElement(new Actor(this), HUD::TEXT_BOX);
	endMessage_text->SetPosition(Vector3(0.0f, 200.0f, 0.0f));

	replayMessage_text = (TextBox*)hud->addElement(new Actor(this), HUD::TEXT_BOX);
	replayMessage_text->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	
}

void Game::UnloadHud()
{
	if (hud)
	{
		hud->clearHUD();
		delete hud;
	}
}