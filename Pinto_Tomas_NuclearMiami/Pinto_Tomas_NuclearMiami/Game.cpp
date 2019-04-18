#include "pch.h"
#include "Game.h"

#include <gl/GLU.h>

#include <iostream>
#include <algorithm>
#include "Game.h"
#include "Core.h"
#include "structs.h"
#include "Matrix2x3.h"

#include "SoundEffect.h"
#include "SoundStream.h"

#include "Ui/TUiManager.h"
#include "Ui/TUiButton.h"
#include "Ui/TUiContainer.h"

#include "GameObject.h"
#include "Player.h"
#include "AiAgent.h"
#include "Scene.h"
#include "Camera.h"
#include "Projectile.h"
#include "Interfaces.h"
#include "PickUp.h"
#include "ResourceManager.h"

Game::Game(const Window& window)
	: m_Window(window)
	//, m_pCamera(new Camera(320.f, 180.f, &m_Window, &m_MousePosition))
	//, m_pCamera(new Camera(640.f, 360.f, &m_Window, &m_MousePosition))
	, m_pCamera(new Camera(480.f, 270.f, &m_Window, &m_MousePosition))
{
	Initialize();
}

Game::~Game()
{
	Cleanup();
}

// TODO(tomas): Think about how i wanna do the background of the menu, once we have the scene working i can make a small scene with just the ai agents going abouts
// TODO(tomas): scene reset
// TODO(tomas): scene parser, right now we just load the texture and svg. Populating with items is done by hand. We want a parser where we can populate the scene in game, export it and then be able to load it back. This parser will work with TScript :)
// TODO(tomas): different enemies
// TODO(tomas): weapon rating system
// TODO(tomas): if ai doesnt have a weapon it prioritizes looking for one unless player is close to him. if enemie is not in sight ai will not prioritize looking for a weapon.
// TODO(tomas): modern opengl, i want to use shaders plsssss, menu fadeaway in to game
// TODO(tomas): remake camera class, implemente screen shape, lerp follow, 
// TODO(tomas): ui slider, ui tick box
// TODO(tomas): more custom AI :D
// TODO(tomas): make a document describing the inheritance of the game
// TODO(tomas): pickups should be reworked to work with every one, ie: every object that has an inventory should have IInventory(void ProcessPickUp(GameObject* pPickup))
// TODO(tomas): when dropping and picking up in the same frame Z-Order is wrong for dropped weapon
// TODO(tomas): organize the header files and cpp files for every class (player, gameobject, etc, ...)
// TODO(tomas): Player and AiAgent share a lot of the same behaviour, maybe put some of that joint behaviour in to a base class? 
void Game::Initialize()
{
	// Startup timer
	std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

	//SDL_ShowCursor(SDL_DISABLE);

	// Load UI from the menu tankscript file and setup callbacks
	TUiManager::Get()->LoadUiDescriptor("Resources/Scripts/menu.ts");
	UiCallbackSetUp();

	// Process preloaded textures
	TUiManager::Get()->LoadUiDescriptor("Resources/Scripts/preload.ts");

	// Initialize singletone instance
	m_pScene = Scene::Get();
	m_pScene->Initialize();
	m_pScene->SetMainCamera(m_pCamera);

	m_ScreenState = ScreenState::MainMenu;

	// Play start menu music
	m_pMenuMusic = ResourceManager::Get()->GetSoundStream("menu_music");

	if (m_pMenuMusic->IsLoaded())
	{
		m_pMenuMusic->SetVolume(15);
		m_pMenuMusic->Play(true);
	}

	// Get Elapsed time
	std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();

	// Calculate elapsed time
	float elapsedSeconds = std::chrono::duration<float>(t2 - t1).count();

	// Prevent jumps in time caused by break points
	LOG("Startup took: " << elapsedSeconds << " seconds...");

}

void Game::Cleanup()
{
	// Singleton memory cleanup
	delete TUiManager::Get();
	delete ResourceManager::Get();
	delete Scene::Get();

	// Normal memory cleanup
	delete m_pCamera;
}

void Game::Update(float dt)
{
	m_ElapsedTime += dt;

	// Update Ui
	TUiManager::Get()->Update(dt, m_MousePosition);

	// Don't update if the game is paused, ui still gets updated
	if (m_ScreenState != ScreenState::Paused &&  m_ScreenState != ScreenState::MainMenu)
		m_pScene->Update(dt);
}

void Game::Draw() const
{
	ClearBackground();

	if (m_ScreenState != ScreenState::Paused &&  m_ScreenState != ScreenState::MainMenu)
	{
		// Calculate camera position with mouse
		Player* pPlayer = m_pScene->GetPlayer();
		Camera* pCamera = m_pScene->GetMainCamera();

		Point2f cameraPosition = pCamera->GetPosition(pPlayer->GetPosition());
		Point2f mouseOffset = Vector2f{ pPlayer->GetPosition().ToPoint2f(), pCamera->GetMouseWS(pPlayer->GetPosition()) }.ToPoint2f();

		cameraPosition.x += mouseOffset.x / 2.f;
		cameraPosition.y += mouseOffset.y / 2.f;

		// Matrix operations
		glPushMatrix();
		glScalef(m_Window.width / pCamera->GetWidth(), m_Window.height / m_pCamera->GetHeight(), 0.f);
		glTranslatef(-cameraPosition.x, -cameraPosition.y, 0.f);

		// DRAW
		m_pScene->Draw();

		// ENDDRAW
		glPopMatrix();
	}

	// Draw Ui
	TUiManager::Get()->Draw(m_Window);
}

void Game::ProcessKeyDownEvent(const SDL_KeyboardEvent & e)
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
	switch (e.keysym.sym)
	{
	case SDLK_t:
		std::cout << GameObject::GetInstanceCount() << std::endl;
		break;

	case SDLK_i:
		ToggleInfo();
		break;

	case SDLK_h:
		m_pScene->GetPlayer()->SendMessage(MessageType::regen, 50);
		break;

	case SDLK_q:
		m_pScene->GetPlayer()->Drop();
		break;

	case SDLK_b:
		m_pScene->AddBlood(m_pScene->GetPlayer()->GetPosition(), 10);
		LOG("Player position: " << m_pScene->GetPlayer()->GetPosition());
		break;
	}
}

void Game::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
	m_MousePosition = Point2f{ float(e.x), float(m_Window.height - e.y) };
}

void Game::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
}

void Game::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
{
}

void Game::ClearBackground() const
{
	if (m_ScreenState != ScreenState::Paused &&  m_ScreenState != ScreenState::MainMenu)
		glClearColor(0.f, 0.f, 0.f, 1.f);
	else
		glClearColor(0.4f, 0.4f, 0.8f, 1.0f);

	//Z-Ordering
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Game::StartGame(std::string level)
{
	m_ScreenState = ScreenState::Playing;
	m_pMenuMusic->Pause();
	// Load level
}

void Game::UnloadGame()
{
	m_ScreenState = ScreenState::MainMenu;
	m_pMenuMusic->Resume();
}

void Game::UiCallbackSetUp()
{
	m_pMenu = TUiManager::Get()->GetComponent<TUiContainer>("menu");

	// Get a pointer to the start button and register the click callback 
	TUiButton* pStartButton = TUiManager::Get()->GetComponent<TUiButton>("menu.startGame");

	if (pStartButton)
	{
		pStartButton->RegisterClickCallBack([&]() {
			TUiContainer* menu = TUiManager::Get()->GetComponent<TUiContainer>("menu");
			if (menu) menu->SetActive(false);
			StartGame("");
		});
	}

	// Settings
	TUiButton* pSettingsButton = TUiManager::Get()->GetComponent<TUiButton>("menu.settingsButton");

	if (pSettingsButton)
	{
		pSettingsButton->RegisterClickCallBack([&]() {
			TUiContainer* settings = TUiManager::Get()->GetComponent<TUiContainer>("settings");
			if (settings) settings->SetActive(true);

			TUiContainer* menu = TUiManager::Get()->GetComponent<TUiContainer>("menu");
			if (menu) menu->SetActive(false);
		});
	}

	// Exit
	TUiButton* pExitButton = TUiManager::Get()->GetComponent<TUiButton>("menu.exitGame");

	if (pExitButton)
	{
		pExitButton->RegisterClickCallBack([&]() {m_ExitFlags->shouldExit = true; });
	}

	// Register click and delta click callbacks for the buttons
	TUiButton* pInfoButton = TUiManager::Get()->GetComponent<TUiButton>("infoButton");

	if (pInfoButton)
	{
		pInfoButton->RegisterClickCallBack([&]() {
			TUiContainer* menu = TUiManager::Get()->GetComponent<TUiContainer>("menu");
			if (menu) menu->SetActive(true);

			TUiContainer* settings = TUiManager::Get()->GetComponent<TUiContainer>("settings");
			if (settings) settings->SetActive(false);

			UnloadGame();
		});

		pInfoButton->RegisterClickDeltaCallBack([](float deltaPressed) {
			TUiContainer* menu = TUiManager::Get()->GetComponent<TUiContainer>("menu.label1");
			if (menu) menu->SetSize(Vector2f{ deltaPressed, 0.5f });

			TUiContainer* info = TUiManager::Get()->GetComponent<TUiContainer>("info");
			if (info) info->SetActive(false);
		});
	}

	// Toggle full screen button
	TUiButton* toggleFullScreenButton = TUiManager::Get()->GetComponent<TUiButton>("settings.fullscreenToggle");

	if (toggleFullScreenButton)
	{
		toggleFullScreenButton->RegisterClickCallBack([&]() {
			m_ExitFlags->isFullScreen = !m_ExitFlags->isFullScreen;

			// Set fullscreen if not in full screen
			if (m_ExitFlags->isFullScreen)
			{
				SDL_SetWindowFullscreen(m_ExitFlags->pWindow, SDL_WINDOW_FULLSCREEN);
				return;
			}

			// Exit fullscreen if in fullscreen
			SDL_SetWindowFullscreen(m_ExitFlags->pWindow, 0);
		});
	}

	TUiButton* toggleVSync = TUiManager::Get()->GetComponent<TUiButton>("settings.vsyncToggle");

	if (toggleVSync)
	{
		toggleVSync->RegisterClickCallBack([&]() {
			if (m_VSync) { SDL_GL_SetSwapInterval(0); m_VSync = false; }
			else { SDL_GL_SetSwapInterval(1); m_VSync = true; }
			LOG("VSync = " << m_VSync);
		});
	}
}

void Game::RaycastVision() const
{
	Player* pPlayer = m_pScene->GetPlayer();

	// Visible
	std::vector<Point2f> visiblePoints;

	for (std::vector<Point2f> collider : m_pScene->GetSceneCollider())
	{
		for (auto pos : collider)
		{
			utils::HitInfo hit;
			
			if (utils::Raycast(collider, pPlayer->GetPosition().ToPoint2f(), pos, hit))
				visiblePoints.push_back(hit.intersectPoint);
		}
	}

	// TODO(tomas): Tesselation with gl/GLU.h
	glColor4f(0.f, 0.f, 0.f, 1.f);
	utils::DrawPolygon(visiblePoints, true, 3.f);
}

void Game::ToggleInfo()
{
	std::cout << "Controls: WASD to move, mouse to aim and shoot. Items are picked up atomatically if there is an empty weapon slot." << std::endl;

	if (m_ScreenState != ScreenState::MainMenu) return;

	TUiContainer* info = TUiManager::Get()->GetComponent<TUiContainer>("info");
	TUiContainer* menu = TUiManager::Get()->GetComponent<TUiContainer>("menu");

	if (info && menu)
	{
		menu->SetActive(info->GetActive());
		info->SetActive(!info->GetActive());
	}
}