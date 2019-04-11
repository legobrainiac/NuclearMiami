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

class Scene
{
public:
	Scene(std::string sceneMapTextureLocation, std::string sceneColliderLocation);
	~Scene();
	
	void Draw() const;
	void Update(float dt);
	
	void Add(GameObject* pGameObject);
	void Delete(GameObject* pGameObject);
	void Remove(GameObject* pGameObject);
	std::vector<GameObject*>& GetGameObjects() { return m_Scene; }
	const std::vector<std::vector<Point2f>>& GetSceneCollider();
	
	Player* GetPlayer() { return m_pPlayer; }
	
	void SetMainCamera(Camera* pCamera);
	void AddBlood(const Vector2f& pos, int ammount);
	Camera* GetMainCamera() const;
	
private:
	
	void DrawBlood() const;
	
	// Container for the basic static level
	struct StaticSceneContainer
	{
		// Texture of the level
		std::string mapTextureLocation;
		Texture* pMapTexture;

		// Point collider for the whole level
		std::string colliderPointSVGLocation;
		std::vector<std::vector<Point2f>> sceneCollider;
	}m_SceneMap;
		
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
	
	void ProcessAdditions();
	void ProcessDeletions();
	void ProcessRemovals();
};

#endif //!SCENE_H