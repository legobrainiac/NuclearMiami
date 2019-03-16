#include "pch.h"
#include "Game.h"

#include <gl/GLU.h>

#include <iostream>
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

Game::Game(const Window& window)
: m_Window(window)
//, m_pCamera(new Camera(320.f, 180.f, &m_Window, &m_MousePosition))
, m_pCamera(new Camera(640.f, 360.f, &m_Window, &m_MousePosition))
{
	m_pScene = new Scene("Resources/Scenes/Scene1/scene1.png", "Resources/Scenes/Scene1/scene1.svg");
	m_pScene->SetMainCamera(m_pCamera);

	Initialize();
}

Game::~Game()
{
	Cleanup();
}

// TODO(tomas): Test collisions, just with the player and the level, if they work like they should implement them directly in to the GameObject class
// TODO(tomas): Think about how i wanna do the background of the menu, once we have the scene working i can make a small scene with just the ai agents going abouts
// TODO(tomas): rule of five for all the ui, generally clean up and bring it up to standars, forgot p prefix for a lot of the points
// TODO(tomas): Implement an active camera thing to allow switching between cameras
// TODO(tomas): Before implementing actual weapons, add an enemy : public AiAgent class that shoots the same basic way to player shoots right now
// TODO(tomas): finish collisions on player and then move it as a default behaviour on to GameObject base class
void Game::Initialize()
{
    // Load UI from the menu tankscript file
	TUiManager::Get().LoadUiDescriptor("Resources/Scripts/menu.ts");
	UiCallbackSetUp();
	m_ScreenState = ScreenState::MainMenu;
	
	// Play start menu music
	m_pMenuMusic = new SoundStream("Resources/Audio/music.wav");
	
	if(m_pMenuMusic->IsLoaded())
	{
		m_pMenuMusic->SetVolume(15);
		m_pMenuMusic->Play(true);
	}
}

void Game::Cleanup()
{
	delete &TUiManager::Get();
	delete m_pMenuMusic;
	delete m_pScene;
	delete m_pCamera;
}

void Game::Update(float dt)
{
	m_ElapsedTime += dt;
	TUiManager::Get().Update(dt, m_MousePosition);
	
	// Don't update if the game is paused, ui still gets updated
	if(m_ScreenState != ScreenState::Paused &&  m_ScreenState != ScreenState::MainMenu)
	{
		// Update game in here
		m_pScene->Update(dt);
	}
}

void Game::Draw() const
{
	ClearBackground();

	if(m_ScreenState != ScreenState::Paused &&  m_ScreenState != ScreenState::MainMenu)
	{
		// Calculate camera position with mouse
		Player* pPlayer = m_pScene->GetPlayer();
		Camera* pCamera = m_pScene->GetMainCamera();
		
		Point2f cameraPosition = pCamera->GetPosition(pPlayer->GetPosition());	
		Point2f mouseOffset = Vector2f {pPlayer->GetPosition().ToPoint2f(), pCamera->GetMouseWS(pPlayer->GetPosition())}.ToPoint2f();
		
		cameraPosition.x += mouseOffset.x / 2.f;
		cameraPosition.y += mouseOffset.y / 2.f;
		
		// Matrix operations
		glPushMatrix();
		glScalef(m_Window.width / pCamera->GetWidth(), m_Window.height /  m_pCamera->GetHeight(), 0.f);
		glTranslatef(-cameraPosition.x, -cameraPosition.y, 0.f);

		// DRAW
		m_pScene->Draw();
		RaycastVision();
		
		// ENDDRAW
		glPopMatrix();
	}
	TUiManager::Get().Draw(m_Window);
}

void Game::ProcessKeyDownEvent(const SDL_KeyboardEvent & e)
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
	switch (e.keysym.sym)
	{
		case SDLK_w:
		break;
	}
}

void Game::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
	m_MousePosition = Point2f{ float(e.x), float(m_Window.height - e.y) };
}

void Game::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	switch (e.button)
	{
		case SDL_BUTTON_LEFT:
		
		break;
	}
}

void Game::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground() const
{
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
	m_pMenu = TUiManager::Get().GetComponent<TUiContainer>("menu");
	
	// Get a pointer to the start button and register the click callback 
	TUiButton* pStartButton = TUiManager::Get().GetComponent<TUiButton>("menu.startGame");
	
	if (pStartButton)
	{
		pStartButton->RegisterClickCallBack(
			[&](){
			auto menu = TUiManager::Get().GetComponent<TUiContainer>("menu");
			if(menu) menu->SetActive(false);
			StartGame("");
			}
			);
	}
	
	// Settings
	TUiButton* pSettingsButton = TUiManager::Get().GetComponent<TUiButton>("menu.settingsButton");
	
	if (pSettingsButton)
	{
		pSettingsButton->RegisterClickCallBack(
			[&](){
			auto settings = TUiManager::Get().GetComponent<TUiContainer>("settings");
			if(settings) settings->SetActive(true);
			
			auto menu = TUiManager::Get().GetComponent<TUiContainer>("menu");
			if(menu) menu->SetActive(false);
			});
		
	}
	
	// Exit
	TUiButton* pExitButton = TUiManager::Get().GetComponent<TUiButton>("menu.exitGame");
	
	if (pExitButton)
	{
		pExitButton->RegisterClickCallBack([&](){m_ExitFlags->shouldExit = true;});
	}
	
	// Register click and delta click callbacks for the buttons
	TUiButton* pInfoButton = TUiManager::Get().GetComponent<TUiButton>("infoButton");
	
	if(pInfoButton)
	{
		pInfoButton->RegisterClickCallBack(
			[&](){
			auto menu = TUiManager::Get().GetComponent<TUiContainer>("menu");
			if(menu) menu->SetActive(true);
			
			auto settings = TUiManager::Get().GetComponent<TUiContainer>("settings");
			if(settings) settings->SetActive(false);
			
			UnloadGame();
			}
			);
		
		pInfoButton->RegisterClickDeltaCallBack(
			[](float deltaPressed){
			auto menu = TUiManager::Get().GetComponent<TUiContainer>("menu.label1");
			if(menu) menu->SetSize(Vector2f {deltaPressed, 0.5f});
			} 
			);
	}
	
	// Toggle full screen button
	TUiButton* toggleFullScreenButton = TUiManager::Get().GetComponent<TUiButton>("settings.fullscreenToggle");
	
	if(toggleFullScreenButton)
	{
		toggleFullScreenButton->RegisterClickCallBack(
			[&](){
			m_ExitFlags->isFullScreen = !m_ExitFlags->isFullScreen;
			
			// Set fullscreen if not in full screen
			if(m_ExitFlags->isFullScreen) 
			{
			SDL_SetWindowFullscreen(m_ExitFlags->pWindow, SDL_WINDOW_FULLSCREEN); 
			return;
			}
			
			// Exit fullscreen if in fullscreen
			SDL_SetWindowFullscreen(m_ExitFlags->pWindow, 0);
			}
			);
	}
}

void Game::RaycastVision() const
{		
	Player* pPlayer = m_pScene->GetPlayer();
	
	// Visible
	std::vector<Point2f> visiblePoints;
		
	for(float i = 0; i <= 2 * PI; i += PI / 180.f)
	{
		float x = std::cosf(i);
		float y = std::sinf(i);
	
		Vector2f dir {x, y};
		dir *= 10000;
		utils::HitInfo hit;
	
		if(utils::Raycast(m_pScene->GetSceneCollider(), pPlayer->GetPosition().ToPoint2f(), dir.ToPoint2f(), hit))
		{
			visiblePoints.push_back(hit.intersectPoint);
		}
		else
		{
			visiblePoints.push_back(pPlayer->GetPosition().ToPoint2f() + dir.ToPoint2f());
		}
	}
	
	glColor4f(0.f, 0.f, 0.f, 1.f);
	/*for(size_t i = 0; i < visiblePoints.size(); ++i)
	{
		Point2f playerPos = m_pPlayer->GetPosition().ToPoint2f();
		utils::DrawLine(playerPos, visiblePoints[i]);
	}*/
	
	// TODO(tomas): Tesselation with gl/GLU.h
	utils::DrawPolygon(visiblePoints, true, 3.f);
	//utils::FillPolygon(visiblePoints);

}