#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <algorithm>

#include "structs.h"

class Texture;
class GameObject;
class Camera;
class Player;

#define WIN_LEVEL 5

struct StaticParticleSystem
{
	int count = 0;
	static const int particleCount = 4096;
	Vector2f particles[particleCount];
	
	void Add(float x, float y)
	{
		particles[count % particleCount] = Vector2f {x, y};
		++count;
	}
};

struct StaticSceneContainer
{
	// Level id
	int level;
	
	// Texture of the level
	std::string mapTextureLocation;
	Texture* pMapTexture;
	
	// Point collider for the whole level
	std::string colliderPointSVGLocation;
	std::vector<std::vector<Point2f>> sceneCollider;
};

// TODO(tomas): rule of five
class Scene
{
public:
	// Mutators
	void Initialize();
	
	// Will reset to the beggining of the game, used when you die or when the player
	//  wishes to restart the game
	void Reset();
	
	// This load the new scene, without deleting the player object.
	// This way we can move on and keep the player inventory for the next level.
	void Load(int scene, Player* pCarry = nullptr);
	
	void Draw() const;
	void Update(float dt);
	
	void SetMainCamera(Camera* pCamera);
	void AddBlood(const Vector2f& pos, int ammount);
	
	void Add(GameObject* pGameObject);
	void Delete(GameObject* pGameObject);
	void Remove(GameObject* pGameObject);
	
	void SetPlayer(Player* pGo) { m_pPlayer = pGo; }
	
	// Getters
	std::vector<GameObject*>& GetGameObjects() { return m_pScene; }
	const std::vector<std::vector<Point2f>>& GetSceneCollider();
	Player* GetPlayer() { return m_pPlayer; }
	Camera* GetMainCamera() const;	
	
	~Scene();
	
	// Don't call this every frame pls
	void ForceSort();
	
	// Singleton impl
	static Scene* Get();
	static int GetLevel() { return m_sLevel; };
	
private:
	void DrawBlood() const;
	void ProcessAdditions();
	void ProcessDeletions();
	void ProcessRemovals();
	void ProcessLoad();
	
	// Scene info
	StaticSceneContainer m_SceneMap;	
	std::vector<GameObject*> m_pScene;
	Vector2f m_PlayerSpawn;
	
	// Objects marked to be added or deleted
    Buffer<GameObject*> m_AddBuffer;
	Buffer<GameObject*> m_DeleteBuffer;
	Buffer<GameObject*> m_RemoveBuffer;
	
	// For loading after the last frame is done updating
	bool m_ShouldLoad;
	int m_NextLevel;
	Player* m_pCarry;
	
	StaticParticleSystem m_BloodPS;
	
	// Game play objects
	Player* m_pPlayer;
	Camera* m_pCamera;	
	
	// Singleton stuff
	Scene(int level);
	static Scene* m_psScene;
	static int m_sLevel;
};

#endif //!SCENE_H