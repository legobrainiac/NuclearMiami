#include "pch.h"

#include "Ui/TUiContainer.h"
#include "Ui/TUiManager.h"
#include "Ui/TUiButton.h"

#include "Matrix2x3.h"
#include "BossFight.h"
#include "AiAgent.h"
#include "Camera.h"
#include "Player.h"

Game::Game(const Window& window)
	: m_Window(window)
	, m_pCamera(new Camera(3.f, &m_Window, &m_MousePosition))
{
	Initialize();
}

Game::~Game()
{
	Cleanup();
}

// TODO(tomas): Boss fight
// TODO(tomas): Melee attack
// TODO(tomas): one more enemie for level two
// TODO(tomas): more custom AI :D
// TODO(tomas): upercase lambdas
// TODO(tomas): instead of having a song for menu and one for playing, each scene can have their own music
void Game::Initialize()
{
	// Startup timer
	std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

	SDL_ShowCursor(SDL_DISABLE);

	m_ScreenState = ScreenState::mainMenu;
	
	// Process preloading
	TUiManager::Get()->LoadUiDescriptor("Resources/Scripts/preload.ts");

	// Load UI from the menu tankscript file and setup callbacks
	TUiManager::Get()->LoadUiDescriptor("Resources/Scripts/menu.ts");
	UiCallbackSetUp();

	// Initialize singletone instance
	m_pScene = Scene::Get();
	m_pScene->Initialize();
	m_pScene->SetMainCamera(m_pCamera);

	// Play start menu music
	m_pMenuMusic = ResourceManager::Get()->GetSoundStream("menu_music");
	m_pGameMusic = ResourceManager::Get()->GetSoundStream("game_music_1");

	if (m_pMenuMusic->IsLoaded())
	{
		m_pMenuMusic->SetVolume(15);
		m_pMenuMusic->Play(true);
	}

	if (m_pGameMusic->IsLoaded())
		m_pGameMusic->SetVolume(128);

	std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
	float elapsedSeconds = std::chrono::duration<float>(t2 - t1).count();
	LOG("Startup took: " << elapsedSeconds << " seconds...");
	
	// Since the scene doesn't get updated until the player starts, if they were to close the game without starting it would create a big memory leak
	m_pScene->ForceProcessAdditions();
	
	TUiContainer* pDeathScreen = TUiManager::Get()->GetComponent<TUiContainer>("deathScreen");
	if (pDeathScreen)
		pDeathScreen->FadeActive(false);
	
	TUiContainer* pEndScreen = TUiManager::Get()->GetComponent<TUiContainer>("endScreen");
	if (pEndScreen)
		pEndScreen->FadeActive(false);
	
	TUiContainer* pSettingsScreen = TUiManager::Get()->GetComponent<TUiContainer>("settings");
	if (pSettingsScreen)
		pSettingsScreen->FadeActive(false);
	
	TUiContainer* pInfoScreen = TUiManager::Get()->GetComponent<TUiContainer>("info");
	if (pInfoScreen)
		pInfoScreen->FadeActive(false);
}

void Game::Cleanup()
{
	// Singleton memory cleanup
	delete Scene::Get();
	delete DebugLogger::Get();
	delete TUiManager::Get();
	delete ResourceManager::Get();

	// Normal memory cleanup
	delete m_pCamera;
} 

void Game::Update(float dt)
{
	m_FrameTime = dt;
	
	DoDeathScreen(dt);
	DoEndScreen(dt);

	// In case a scene reset happens and our pointers become invalid
	m_pScene = Scene::Get();
	m_pScene->SetMainCamera(m_pCamera);

	// Update camera and camera offset
	m_pCamera->Update(dt);	
	Player* pPlayer = m_pScene->GetPlayer();
	Point2f cameraPosition = m_pCamera->GetPosition(pPlayer->GetPosition());
	Point2f m_MouseOffsetGoal = Vector2f{ pPlayer->GetPosition().ToPoint2f(), m_pCamera->GetMouseWS(pPlayer->GetPosition()) }.ToPoint2f();
	m_CurrentMouseOffset = utils::Lerp(Vector2f { m_CurrentMouseOffset }, Vector2f { m_MouseOffsetGoal }, dt * 10.f);
	
	// Update Ui
	TUiManager::Get()->Update(dt, m_MousePosition);

	// Don't update if the game is paused, ui still gets updated
	if (m_ScreenState != ScreenState::paused &&  m_ScreenState != ScreenState::mainMenu)
	{
		m_pScene->Update(dt);

		if(!pPlayer->IsDead() && m_ScreenState != ScreenState::endScreen)
			m_ElapsedTime += dt;
	}
}
	
void Game::DoDeathScreen(float dt)
{
	if (Scene::Get()->GetPlayer()->IsDead() && m_ScreenState != ScreenState::mainMenu)
	{
		m_ScreenState = ScreenState::deathScreen;
		m_EndScreenTimer += dt;

		TUiContainer* pDeathScreen = TUiManager::Get()->GetComponent<TUiContainer>("deathScreen");
		if (pDeathScreen)
			pDeathScreen->FadeActive(true);

		TUiButton* pInfoButton = TUiManager::Get()->GetComponent<TUiButton>("infoButton");
		if (pInfoButton)
			pInfoButton->SetActive(false);
	}

	if (m_EndScreenTimer > 5.f && m_ScreenState == ScreenState::deathScreen)
	{
		Scene::Get()->Reset();
		m_ScreenState = ScreenState::playing;
		m_EndScreenTimer = 0.f;

		TUiContainer* pDeathScreen = TUiManager::Get()->GetComponent<TUiContainer>("deathScreen");
		if (pDeathScreen)
			pDeathScreen->FadeActive(false);

		TUiButton* pInfoButton = TUiManager::Get()->GetComponent<TUiButton>("infoButton");
		if (pInfoButton)
			pInfoButton->SetActive(true);
		
		m_ElapsedTime = 0.f;
	}
}

void Game::DoEndScreen(float dt)
{
	if (AiAgent::GetAiInstanceCount() <= 0 && BossFight::GetBossFightCounter() <= 0 && !Scene::Get()->GetPlayer()->IsDead() && m_ScreenState != ScreenState::mainMenu && Scene::GetLevel() == WIN_LEVEL)
	{
		m_ScreenState = ScreenState::endScreen;
		m_EndScreenTimer += dt;

		TUiContainer* pEndScreen = TUiManager::Get()->GetComponent<TUiContainer>("endScreen");
		if (pEndScreen)
			pEndScreen->FadeActive(true);

		TUiButton* pInfoButton = TUiManager::Get()->GetComponent<TUiButton>("infoButton");
		if (pInfoButton)
			pInfoButton->SetActive(false);
	}

	if (m_EndScreenTimer > 10.f && m_ScreenState == ScreenState::endScreen)
	{
		Scene::Get()->Reset();
		m_ScreenState = ScreenState::playing;
		m_EndScreenTimer = 0.f;

		TUiContainer* pEndScreen = TUiManager::Get()->GetComponent<TUiContainer>("endScreen");
		if (pEndScreen)
			pEndScreen->FadeActive(false);

		TUiButton* pInfoButton = TUiManager::Get()->GetComponent<TUiButton>("infoButton");
		if (pInfoButton)
			pInfoButton->SetActive(true);
		
		m_ElapsedTime = 0.f;
	}
}

void Game::Draw() const
{
	ClearBackground();

	// Calculate camera position with mouse
	m_pScene->SetMainCamera(m_pCamera);
	Player* pPlayer = m_pScene->GetPlayer();
	Camera* pCamera = m_pScene->GetMainCamera();

	Point2f cameraPosition = pCamera->GetPosition(pPlayer->GetPosition());
		
	cameraPosition.x += m_CurrentMouseOffset.x / 2.f;
	cameraPosition.y += m_CurrentMouseOffset.y / 2.f;
		
	// Matrix operations
	glPushMatrix();
	pCamera->Transform(cameraPosition);

	// DRAW
	m_pScene->Draw();
	
	// ENDDRAW
	glPopMatrix();

	// Draw Ui
	TUiManager::Get()->Draw(m_Window);
	
	// Frame time render
	TextRenderConfig textConfig;
	textConfig.spacing = 3.f;
	textConfig.scale = 1.f;
	
	if(GameObject::GetDebug())
	{
		int fps = int(1.f / m_FrameTime);
		ResourceManager::Get()->GetTextRenderer("munroDebug")->DrawString(std::to_string(fps), Vector2f { 10.f, m_Window.height - 40.f }, textConfig);
		DebugLogger::Get()->Draw();
	}

	textConfig.scale = 2.f;
	glPushMatrix();
	glTranslatef(m_MousePosition.x, m_MousePosition.y, 0.f);
	ResourceManager::Get()->GetTextRenderer("munro")->DrawString("+", Vector2f { -10.f, -17.f }, textConfig);
	glPopMatrix();

	// Draw timer
	if(m_ScreenState != ScreenState::mainMenu)
		ResourceManager::Get()->GetTextRenderer("munro")->DrawString("Time: " + std::to_string(m_ElapsedTime), Vector2f { 10.f, 320.f }, textConfig);
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
		break;

	case SDLK_q:
		m_pScene->GetPlayer()->Drop();
		break;

	case SDLK_b:
		LDEBUG("Player position: " + Scene::Get()->GetPlayer()->GetPosition().ToString());
		break;

	case SDLK_x:
		if(m_ScreenState != ScreenState::deathScreen && m_ScreenState != ScreenState::endScreen)
		{
			Scene::Get()->Reset();
			m_ElapsedTime = 0.f;
		}
		
		break;
		
	case SDLK_BACKSPACE:
		DebugLogger::Get()->Clear();
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
	glClearColor(0.f, 0.f, 0.f, 1.f);

	//Z-Ordering
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Game::StartGame(const std::string& level)
{
	m_ScreenState = ScreenState::playing;
	m_pMenuMusic->Stop();
	m_pGameMusic->Play(true);
}

void Game::UnloadGame()
{
	m_ScreenState = ScreenState::mainMenu;
	m_pGameMusic->Stop();
	m_pMenuMusic->Play(true);
}

void Game::UiCallbackSetUp()
{
	m_pMenu = TUiManager::Get()->GetComponent<TUiContainer>("menu");

	// Get a pointer to the start button and register the click callback 
	TUiButton* pStartButton = TUiManager::Get()->GetComponent<TUiButton>("menu.startGame");
	if (pStartButton)
	{
		auto Callback = [this]() 
		{
			TUiContainer* pMenu = TUiManager::Get()->GetComponent<TUiContainer>("menu");
			if (pMenu) pMenu->FadeActive(false);
			
			TUiContainer* pHud = TUiManager::Get()->GetComponent<TUiContainer>("HUD");
			if (pHud) pHud->FadeActive(true);
			StartGame("");
		};
		
		pStartButton->RegisterClickCallBack(Callback);
	}

	// Settings
	TUiButton* pSettingsButton = TUiManager::Get()->GetComponent<TUiButton>("menu.settingsButton");
	if (pSettingsButton)
	{
		auto Callback = [this]() 
		{
			TUiContainer* pSettings = TUiManager::Get()->GetComponent<TUiContainer>("settings");
			if (pSettings) pSettings->FadeActive(true);
			
			TUiContainer* pMenu = TUiManager::Get()->GetComponent<TUiContainer>("menu");
			if (pMenu) pMenu->FadeActive(false);
		};
		
		pSettingsButton->RegisterClickCallBack(Callback);
	}

	// Exit
	TUiButton* pExitButton = TUiManager::Get()->GetComponent<TUiButton>("menu.exitGame");
	if (pExitButton)
		pExitButton->RegisterClickCallBack([this]() { m_ExitFlags->shouldExit = true; });

	// Register click and delta click callbacks for the buttons
	TUiButton* pInfoButton = TUiManager::Get()->GetComponent<TUiButton>("infoButton");
	if (pInfoButton)
	{
		auto Callback = [this]() 
		{
			TUiContainer* pHud = TUiManager::Get()->GetComponent<TUiContainer>("HUD");
			if (pHud) pHud->FadeActive(false);
			
			TUiContainer* pMenu = TUiManager::Get()->GetComponent<TUiContainer>("menu");
			if (pMenu) pMenu->FadeActive(true);
			
			TUiContainer* pSettings = TUiManager::Get()->GetComponent<TUiContainer>("settings");
			if (pSettings) pSettings->FadeActive(false);
			
			UnloadGame();
		};
		
		pInfoButton->RegisterClickCallBack(Callback);
	}

	// Toggle full screen button
	TUiButton* pToggleFullScreenButton = TUiManager::Get()->GetComponent<TUiButton>("settings.fullscreenToggle");
	if (pToggleFullScreenButton)
	{
		auto Callback = [this]() 
		{
			m_ExitFlags->isFullScreen = !m_ExitFlags->isFullScreen;
			
			// Set fullscreen if not in full screen
			if (m_ExitFlags->isFullScreen)
			{
				SDL_SetWindowFullscreen(m_ExitFlags->pWindow, SDL_WINDOW_FULLSCREEN);
				return;
			}
			
			// Exit fullscreen if in fullscreen
			SDL_SetWindowFullscreen(m_ExitFlags->pWindow, 0);
		};
		
		pToggleFullScreenButton->RegisterClickCallBack(Callback);
	}

	TUiButton* pToggleVSync = TUiManager::Get()->GetComponent<TUiButton>("settings.vsyncToggle");
	if (pToggleVSync)
	{
		auto Callback = [this]() 
		{
			if (m_VSync) 
			{ 	
				SDL_GL_SetSwapInterval(0); 
				m_VSync = false; 
				return;
			} 
			
			SDL_GL_SetSwapInterval(1); 
			m_VSync = true; 
			LOG("VSync = " << m_VSync);
		};
		
		pToggleVSync->RegisterClickCallBack(Callback);
	}
	
	TUiButton* pToggleDebug = TUiManager::Get()->GetComponent<TUiButton>("settings.goDebug");
	if (pToggleDebug)
		pToggleDebug->RegisterClickCallBack([&]() { GameObject::ToggleDebug(); });

	TUiButton* pToggleGodMode = TUiManager::Get()->GetComponent<TUiButton>("settings.toggleGodMode");
	if (pToggleGodMode)
		pToggleGodMode->RegisterClickCallBack([&]() { m_pScene->GetPlayer()->ToggleGodMode(); });
}

void Game::ToggleInfo()
{
	std::cout << "Controls: WASD to move, mouse to aim and shoot, Q to drop weapons. Items are picked up automatically if there is an empty weapon slot." << std::endl;

	TUiContainer* pInfo = TUiManager::Get()->GetComponent<TUiContainer>("info");
	if (pInfo)
	{
		m_Info = !m_Info;
		pInfo->FadeActive(m_Info);
	}
}
