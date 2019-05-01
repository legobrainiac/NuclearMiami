#pragma once

#include "Core.h"
#include "structs.h"

class Core;
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
    
    void SetExitFlags(CoreFlags* exitFlags) { m_ExitFlags = exitFlags; }
    
    private:
	// DATA MEMBERS
	Window m_Window;
	Point2f m_MousePosition;
	bool m_VSync = true;
    
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
		MainMenu, // State for the main menu when not playing
		Playing,// State for when playing
		Paused, // State for menu when playing
		EndScreen, // State for when the player wins
		DeathScreen // State for when the player wins
	}m_ScreenState;
	
	// Game start
	void StartGame(const std::string& level);
	void UnloadGame();
	
	void RaycastVision() const;
	
	// Helpers
	void ToggleInfo();
	
	void DoDeathScreen(float dt);
	void DoEndScreen(float dt);
};