#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <algorithm>

#include "structs.h"

class Texture;
class GameObject;
class Camera;
class Player;

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
	std::vector<GameObject*>& GetGameObjects() { return m_Scene; } // TODO(tomas): do this in cpp file
	const std::vector<std::vector<Point2f>>& GetSceneCollider();
	
	Player* GetPlayer() { return m_pPlayer; }
	
	void SetMainCamera(Camera* pCamera);
	Camera* GetMainCamera() const;
	
private:
	
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
	
	// Game play objects
	Player* m_pPlayer;
	Camera* m_pCamera;
	
	void ProcessAdditions();
	void ProcessDeletions();
	void ProcessRemovals();
};

#endif //!SCENE_H