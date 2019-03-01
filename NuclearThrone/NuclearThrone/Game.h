#pragma once

#include "Core.h"
#include "structs.h"

class Core;
class SoundEffect;
class SoundStream;

class Game
{
public:
	explicit Game(const Window& window);
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	~Game();
    
	void Update(float elapsedSec);
	void Draw() const;
    
	// Event handling
	void ProcessKeyDownEvent(const SDL_KeyboardEvent& e);
	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e);
	void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e);
	void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e);
	void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e);
    
    void SetExitFlags(ExitFlags* exitFlags) { m_ExitFlags = exitFlags; }
    
    private:
	// DATA MEMBERS
	Window m_Window;
	Point2f m_MousePosition;
    
	TUiNode* m_pMenu;
	ExitFlags* m_ExitFlags;
    float m_ElapsedTime;
	
	// Sound
	SoundStream* m_pMenuMusic;
	
	// FUNCTIONS
	void Initialize();
	void Cleanup();
	void ClearBackground() const;
	
	void UiCallbackSetUp();
	
	enum ScreenState
	{
		MainMenu, // State for the main menu when not playing
		Playing,// State for when playing
		Paused // State for menu when playing
	}m_ScreenState;
	
	// Game start
	void StartGame(std::string level);
	void UnloadGame();
};