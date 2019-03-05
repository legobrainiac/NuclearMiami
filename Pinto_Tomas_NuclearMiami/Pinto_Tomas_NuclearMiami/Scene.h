#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <algorithm>

#include "structs.h"

class Texture;
class GameObject;

class Scene
{
public:
	Scene(std::string sceneMapTextureLocation, std::string sceneColliderLocation, GameObject* playerGameObject = nullptr);
	~Scene();
	
	void Draw() const;
	void Update(float dt);
	
	void Add(GameObject* pGameObject);
	const std::vector<Point2f>& GetSceneCollider();
	
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
	
	// GameObject Buffer
	bool m_Dirty;
	std::vector<GameObject*> m_AddBuffer;
};

#endif //!SCENE_H

