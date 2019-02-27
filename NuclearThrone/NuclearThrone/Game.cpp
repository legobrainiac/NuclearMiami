#include "pch.h"
#include "Game.h"

#include <iostream>
#include "Game.h"
#include "Core.h"
#include "structs.h"
#include "Ui/TUiManager.h"
#include "Ui/TUiButton.h"
#include "Ui/TUiContainer.h"

Game::Game(const Window& window)
: m_Window{ window }
{
	Initialize();
}

Game::~Game()
{
	Cleanup();
}


// TODO(tomas): GameObject class
// TODO(tomas): Player class
// TODO(tomas): Think about how i wanna do the background of the menu, once we have the scene working i can make a small scene with just the ai agents going abouts
void Game::Initialize()
{
    // Load UI from the menu tankscript file
	TUiManager::Get().LoadUiDescriptor("Resources/Scripts/menu.ts");
	UiCallbackSetUp();
	m_ScreenState = ScreenState::MainMenu;
}

void Game::Cleanup()
{
    delete &TUiManager::Get();
}

void Game::Update(float dt)
{
    m_ElapsedTime += dt;
    TUiManager::Get().Update(dt, m_MousePosition);
	
	// Exit update if the game is paused, ui still gets updated
	if(m_ScreenState == ScreenState::Paused || m_ScreenState == ScreenState::MainMenu) return;
    
    // Check keyboard state
    //const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
    //if ( pStates[SDL_SCANCODE_RIGHT] )
    //{
    //	std::cout << "Right arrow key is down\n";
    //}
    //if ( pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_UP])
    //{
    //	std::cout << "Left and up arrow keys are down\n";
    //}
}

void Game::Draw() const
{
    ClearBackground();
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
        case SDLK_t:
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
    glClear(GL_COLOR_BUFFER_BIT);
}

void Game::StartGame(std::string level)
{
	m_ScreenState = ScreenState::Playing;
	// Load level
}

void Game::UnloadGame()
{
	m_ScreenState = ScreenState::MainMenu;
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
        auto callback = [](){
            auto settings = TUiManager::Get().GetComponent<TUiContainer>("settings");
            if(settings) settings->SetActive(true);
            
            auto menu = TUiManager::Get().GetComponent<TUiContainer>("menu");
            if(menu) menu->SetActive(false);
        };
        
        pSettingsButton->RegisterClickCallBack(callback);
    }
    
	// Exit
    TUiButton* pExitButton = TUiManager::Get().GetComponent<TUiButton>("menu.exitGame");
    
    if (pExitButton)
    {
        auto callback = [&](){
            m_ExitFlags->shouldExit = true;
        };
        
        pExitButton->RegisterClickCallBack(callback);
    }
    
    // Register click and delta click callbacks for the buttons
    TUiButton* pInfoButton = TUiManager::Get().GetComponent<TUiButton>("infoButton");
    
    if(pInfoButton)
    {
        pInfoButton->RegisterClickCallBack(
            [](){
            auto menu = TUiManager::Get().GetComponent<TUiContainer>("menu");
            if(menu) menu->SetActive(true);
            
            auto settings = TUiManager::Get().GetComponent<TUiContainer>("settings");
            if(settings) settings->SetActive(false);
            }
            );
        
        pInfoButton->RegisterClickDeltaCallBack(
            [](float deltaPressed){
            auto menu = TUiManager::Get().GetComponent<TUiContainer>("menu.label1");
            if(menu) menu->SetSize(Vector2f {deltaPressed, 0.5f});
            } 
            );
    }
}