#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <algorithm>

#include "structs.h"

class Texture;
class GameObject;
class Camera;
class Player;

#define PS_SIZE 4096

struct StaticParticleSystem
{
	int count = 0;
	Vector2f particles[PS_SIZE];
	
	void Add(float x, float y)
	{
		particles[count % PS_SIZE] = Vector2f {x, y};
		++count;
	}
};

struct StaticSceneContainer
{
	// Texture of the level
	std::string mapTextureLocation;
	Texture* pMapTexture;
	
	// Point collider for the whole level
	std::string colliderPointSVGLocation;
	std::vector<std::vector<Point2f>> sceneCollider;
};

class Scene
{
public:
	// Mutators
	void Initialize();
	void Draw() const;
	void Update(float dt);
	
	void SetMainCamera(Camera* pCamera);
	void AddBlood(const Vector2f& pos, int ammount);
	
	void Add(GameObject* pGameObject);
	void Delete(GameObject* pGameObject);
	void Remove(GameObject* pGameObject);
	
	// Getters
	std::vector<GameObject*>& GetGameObjects() { return m_Scene; }
	const std::vector<std::vector<Point2f>>& GetSceneCollider();
	Player* GetPlayer() { return m_pPlayer; }
	Camera* GetMainCamera() const;	
	
	~Scene();
	
	// Singleton impl
	static Scene* Get();
	
private:
	void DrawBlood() const;
	void ProcessAdditions();
	void ProcessDeletions();
	void ProcessRemovals();
	
	// Scene info
	StaticSceneContainer m_SceneMap;	
	std::vector<GameObject*> m_Scene;
	Vector2f m_PlayerSpawn;
	
	// Objects marked to be added or deleted
    Buffer<GameObject*> m_AddBuffer;
	Buffer<GameObject*> m_DeleteBuffer;
	Buffer<GameObject*> m_RemoveBuffer;
	
	StaticParticleSystem m_BloodPS;
	
	// Game play objects
	Player* m_pPlayer;
	Camera* m_pCamera;	
	
	// Singleton stuff
	Scene(std::string sceneMapTextureLocation, std::string sceneColliderLocation);
	static Scene* m_psScene;
};

#endif //!SCENE_H