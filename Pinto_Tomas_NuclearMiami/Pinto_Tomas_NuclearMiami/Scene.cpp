#include "pch.h"

#include "Scene.h"
#include "Texture.h"
#include "GameObject.h"
#include "SVGParser.h"
#include "Player.h"
#include "AiAgent.h"
#include "PickUp.h"
#include "Weapon.h"

Scene::Scene(std::string sceneMapTextureLocation, std::string sceneColliderLocation)
: m_Scene()
, m_PlayerSpawn()
, m_AddBuffer()
{
	m_SceneMap.mapTextureLocation = sceneMapTextureLocation;
	m_SceneMap.colliderPointSVGLocation = sceneColliderLocation;
	
	m_SceneMap.pMapTexture = new Texture(sceneMapTextureLocation);
	
	if(SVGParser::GetVerticesFromSvgFile(m_SceneMap.colliderPointSVGLocation, m_SceneMap.sceneCollider))
	{
		std::cout << "Loaded collider for scene at " << m_SceneMap.colliderPointSVGLocation << std::endl; 
	}
	
	// Test objects in scene
	m_pPlayer = new Player(Vector2f{200.f, 300.f},Vector2f{1.f, 1.f}, 0.f, this);
	
	AiAgent* aiAgentTest = new AiAgent(Vector2f { 300.f, 300.f }, Vector2f { 1.f, 1.f }, 0.f, m_pPlayer, this);
	aiAgentTest->SetZLayer(-1.f);

	AiAgent* aiAgentTest2 = new AiAgent(Vector2f { 600.f, 100.f }, Vector2f { 1.f, 1.f }, 0.f, m_pPlayer, this);
	aiAgentTest2->SetZLayer(-1.f);
	
	PickUp* pickUp1 = new PickUp(Vector2f {200.f, 150.f}, Vector2f {1.f, 1.f}, 25.f, m_pPlayer);
	
	Weapon* pickUp2 = new Weapon(Vector2f {230.f, 120.f}, Vector2f {1.f, 1.f}, 61.f, m_pPlayer, "Resources/Images/Weapons/Rifle.png", 50.f);
	
	Weapon* pickUp3 = new Weapon(Vector2f {630.f, 130.f}, Vector2f {1.f, 1.f}, 74.f, m_pPlayer, "Resources/Images/Weapons/Rifle.png", 50.f);
	
	pickUp1->SetZLayer(-2);
	pickUp2->SetZLayer(-2);
	pickUp3->SetZLayer(-2);
	
	Add(m_pPlayer);
	Add(aiAgentTest);
	Add(aiAgentTest2);
	Add(pickUp1);
	Add(pickUp2);
	Add(pickUp3);
}

void Scene::Draw() const
{	
	glPushMatrix();
	glScalef(1.f, 1.f, 0.f);
	glTranslatef(0.f, 0.f, 0.f);

	m_SceneMap.pMapTexture->Draw(Point2f {0.f, 0.f}, Rectf {0.f, 0.f, m_SceneMap.pMapTexture->GetWidth(),  m_SceneMap.pMapTexture->GetHeight()});
	
	//utils::DrawPolygon(m_SceneMap.sceneCollider[0], true, 2.f);
	
	for(GameObject* go : m_Scene)
		go->Draw();
	
	glPopMatrix();
}

void Scene::Update(float dt)
{
	for(GameObject* go : m_Scene)
		go->Update(dt);
	
	// After updating, 
	ProcessAdditions();
	ProcessDeletions();
	ProcessRemovals();
}

void Scene::Add(GameObject* pGameObject)
{
	if(pGameObject != nullptr)
		m_AddBuffer.Add(pGameObject);
}

// Marks objects for removal and deletion
void Scene::Delete(GameObject* pGameObject)
{
	if(pGameObject != nullptr)
	{
		pGameObject->MakeDirty();
		m_DeleteBuffer.Add(pGameObject);
	}
}

// Removes object from scene
// NOTE(tomas): If this object is not taken in by another object it will cause a memory leak
void Scene::Remove(GameObject* pGameObject)
{
	if(pGameObject != nullptr)
		m_RemoveBuffer.Add(pGameObject);
}

const std::vector<Point2f>& Scene::GetSceneCollider()
{
	return m_SceneMap.sceneCollider[0];
}

void Scene::SetMainCamera(Camera* pCamera)
{
	m_pCamera = pCamera;
}

Camera* Scene::GetMainCamera() const
{
	return m_pCamera;
}

Scene::~Scene()
{
	for(GameObject* go : m_Scene)
		delete go;
	
	delete m_SceneMap.pMapTexture;
}

void Scene::ProcessAdditions()
{
	if(m_AddBuffer.dirty)
	{
		for(GameObject* go : m_AddBuffer.buffer)
			m_Scene.push_back(go);
		
		m_AddBuffer.Reset();
		
		auto sort = [](GameObject* left, GameObject* right) 
		{
			return left->GetZLayer() < right->GetZLayer();
		};
		
		std::sort(m_Scene.begin(), m_Scene.end(), sort);
	}
}

void Scene::ProcessDeletions()
{
	if(m_DeleteBuffer.dirty)
	{
		size_t objectCount = m_DeleteBuffer.buffer.size();
		m_DeleteBuffer.Reset();
		
		auto sort = [](GameObject* left, GameObject* right) 
		{
			return left->IsDirty() < right->IsDirty();
		};
		
		std::sort(m_Scene.begin(), m_Scene.end(), sort);
		for(size_t i = 0; i < objectCount; ++i)
		{
			delete m_Scene.back();
			m_Scene.pop_back();
		}
	}
}

void Scene::ProcessRemovals()
{
	if(m_RemoveBuffer.dirty)
	{
		for(GameObject* go : m_RemoveBuffer.buffer)
		{
			for(size_t i = 0; i < m_Scene.size(); ++i)
			{
				if(go == m_Scene[i])
				{
					GameObject* last = m_Scene.back();
					m_Scene[m_Scene.size() - 1] = m_Scene[i];
					m_Scene[i] = last;
					m_Scene.pop_back();
				}
			}
		}
		
		m_RemoveBuffer.Reset();
	}
}