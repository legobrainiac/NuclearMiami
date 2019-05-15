#pragma once

#include "Core.h"
#include "structs.h"

class Core;
class TUiNode;
class SoundEffect;
class SoundStream;
class GameObject;
class Player;
class Scene;
class Camera;

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
    
    void SetExitFlags(CoreFlags* pExitFlags) { m_ExitFlags = pExitFlags; }
    
    private:
	// DATA MEMBERS
	Window m_Window;
	Point2f m_MousePosition;
	bool m_VSync = true;
    
	Vector2f m_CurrentMouseOffset;
	Vector2f m_MouseOffsetGoal;
	
	TUiNode* m_pMenu;
	CoreFlags* m_ExitFlags;
    float m_ElapsedTime;
	float m_EndScreenTimer;
	float m_FrameTime;

	// Sound
	SoundStream* m_pMenuMusic;
	SoundStream* m_pGameMusic;
	// Scene
	Scene* m_pScene;
	Camera* m_pCamera;
	
	// FUNCTIONS
	void Initialize();
	void Cleanup();
	void ClearBackground() const;
	
	void UiCallbackSetUp();
	
	//TODO(tomas): Lower case
	enum class ScreenState
	{
		mainMenu, // State for the main menu when not playing
		playing,// State for when playing
		paused, // State for menu when playing
		endScreen, // State for when the player wins
		deathScreen // State for when the player wins
	}m_ScreenState;
	
	// Game start
	void StartGame(const std::string& level);
	void UnloadGame();
	
	// Helpers
	void ToggleInfo();
	
	void DoDeathScreen(float dt);
	void DoEndScreen(float dt);
};