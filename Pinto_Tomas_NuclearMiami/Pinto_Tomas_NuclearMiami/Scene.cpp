#include "pch.h"

#include "Scene.h"
#include "Texture.h"
#include "GameObject.h"

Scene::Scene(std::string sceneMapTextureLocation, std::string sceneColliderLocation, GameObject* playerGameObject)
: m_Scene()
, m_PlayerSpawn()
{
	m_SceneMap.mapTextureLocation = sceneMapTextureLocation;
	m_SceneMap.colliderPointCSVLocation = sceneColliderLocation;
	
	m_SceneMap.pMapTexture = new Texture(sceneMapTextureLocation);
	
	if(playerGameObject) Add(playerGameObject);
}

void Scene::Draw() const
{	
	glPushMatrix();
	glScalef(3.f, 3.f, 0.f);
	glTranslatef(-30.f, -200.f, 0.f);

	m_SceneMap.pMapTexture->Draw(Point2f {0.f, 0.f}, Rectf {0.f, 0.f, m_SceneMap.pMapTexture->GetWidth(),  m_SceneMap.pMapTexture->GetHeight()});
	
	glPopMatrix();
	
	for(auto go : m_Scene)
	{
		go->Draw();
	}
}

void Scene::Update(float dt)
{
	for(auto go : m_Scene)
	{
		go->Update(dt);
	}
}

void Scene::Add(GameObject* pGameObject)
{
	m_Scene.push_back(pGameObject);

	// Sort condition
	auto sort = [](GameObject* left, GameObject* right) 
	{
		return left->GetZLayer() < right->GetZLayer();
	};
	
	std::sort(m_Scene.begin(), m_Scene.end(), sort);
}

Scene::~Scene()
{
	for(GameObject* go : m_Scene)
	{
		delete go;
	}
	
	delete m_SceneMap.pMapTexture;
}
