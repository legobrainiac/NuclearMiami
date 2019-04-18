#include "pch.h"

#include "Scene.h"
#include "Texture.h"
#include "GameObject.h"
#include "SVGParser.h"
#include "Player.h"
#include "AiAgent.h"
#include "PickUp.h"
#include "Weapon.h"

Scene* Scene::m_psScene = nullptr;

Scene* Scene::Get()
{
	if(!m_psScene)
		m_psScene = new Scene("Resources/Scenes/Scene3/scene3.png", "Resources/Scenes/Scene3/scene3.svg"); // NOTE(tomas): Temporary
	
	return m_psScene;
}

Scene::Scene(std::string sceneMapTextureLocation, std::string sceneColliderLocation)
: m_Scene()
, m_PlayerSpawn()
, m_AddBuffer()
{
	m_SceneMap.mapTextureLocation = sceneMapTextureLocation;
	m_SceneMap.colliderPointSVGLocation = sceneColliderLocation;
	
	m_SceneMap.pMapTexture = new Texture(sceneMapTextureLocation);
	
	if(SVGParser::GetVerticesFromSvgFile(m_SceneMap.colliderPointSVGLocation, m_SceneMap.sceneCollider))
		LOG("Loaded collider for scene at " << m_SceneMap.colliderPointSVGLocation); 
}

void Scene::Initialize()
{
	// TODO(tomas): TUiManager preloader tokens please
	
	// Populate scene
	m_pPlayer = new Player(Vector2f{200.f, 300.f},Vector2f{1.f, 1.f}, 0.f);
	
	AiAgent* aiAgentTest = new AiAgent(Vector2f { 280.f, 700.f }, Vector2f { 1.f, 1.f }, 0.f, m_pPlayer);
	aiAgentTest->SetZLayer(-1.f);

	AiAgent* aiAgentTest2 = new AiAgent(Vector2f { 600.f, 100.f }, Vector2f { 1.f, 1.f }, 0.f, m_pPlayer);
	aiAgentTest2->SetZLayer(-1.f);
	
	AiAgent* aiAgentTest3 = new AiAgent(Vector2f { 620.f, 90.f }, Vector2f { 1.f, 1.f }, 0.f, m_pPlayer);
	aiAgentTest2->SetZLayer(-1.f);
	
	AiAgent* aiAgentTest4 = new AiAgent(Vector2f { 580.f, 110.f }, Vector2f { 1.f, 1.f }, 0.f, m_pPlayer);
	aiAgentTest2->SetZLayer(-1.f);
	
	AiAgent* aiAgentTest5 = new AiAgent(Vector2f { 630.f, 130.f }, Vector2f { 1.f, 1.f }, 0.f, m_pPlayer);
	aiAgentTest2->SetZLayer(-1.f);
	
	Weapon* pickUp1 = new Weapon(Vector2f {230.f, 130.f}, Vector2f {1.f, 1.f}, 74.f, "rifle", 200, 15);
	
	Weapon* pickUp2 = new Weapon(Vector2f {250.f, 120.f}, Vector2f {1.f, 1.f}, 61.f, "pistol", 50.f, 4);
	
	Weapon* pickUp3 = new Weapon(Vector2f {543.f, 420.f}, Vector2f {1.f, 1.f}, 74.f, "rifle", 200.f, 15);

	pickUp1->SetZLayer(-2);
	pickUp2->SetZLayer(-2);
	pickUp3->SetZLayer(-2);
	
	Add(m_pPlayer);
	Add(aiAgentTest);
	Add(aiAgentTest2);
	Add(aiAgentTest3);
	Add(aiAgentTest4);
	Add(aiAgentTest5);
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
	
	DrawBlood();
	
	for(GameObject* go : m_Scene)
		go->Draw();
	
	glPopMatrix();
}

void Scene::Update(float dt)
{
	for(GameObject* go : m_Scene)
		go->Update(dt);
	
	// After updating, 
	ProcessRemovals();
	ProcessAdditions();
	ProcessDeletions();
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

const std::vector<std::vector<Point2f>>& Scene::GetSceneCollider()
{
	return m_SceneMap.sceneCollider;
}

void Scene::SetMainCamera(Camera* pCamera)
{
	m_pCamera = pCamera;
}

Camera* Scene::GetMainCamera() const
{
	return m_pCamera;
}

void Scene::AddBlood(const Vector2f& pos, int ammount)
{
	for(int i = 0; i < ammount; ++i)
	{
		int rotation = utils::RandInterval(0, 365);
		Vector2f direction = Vector2f { cos(rotation * PI / 180.f), sin(rotation * PI / 180.f) };
		direction *= float(utils::RandInterval(-3, 20)); 
		m_BloodPS.Add(pos.x + direction.x, pos.y + direction.y); 
	}
}

void Scene::DrawBlood() const
{	
	glColor4f(1.f, 0.f, 0.f, 1.f);
	for(int i = 0; i < m_BloodPS.count; ++i)
	{
		glPushMatrix();
		glTranslatef(m_BloodPS.particles[i].x, m_BloodPS.particles[i].y, 0.f);
		
		utils::DrawPoint(0.f, 0.f, 4.f);
		
		glPopMatrix();
	}
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
		
		auto sort = [](GameObject* left, GameObject* right) 
		{
			return left->GetZLayer() < right->GetZLayer();
		};
		
		std::sort(m_Scene.begin(), m_Scene.end(), sort);
		
		m_AddBuffer.Reset();
	}
}

void Scene::ProcessDeletions()
{
	if(m_DeleteBuffer.dirty)
	{
		size_t objectCount = m_DeleteBuffer.buffer.size();
		
		auto sort = [](GameObject* left, GameObject* right) 
		{
			return left->IsDirty() < right->IsDirty();
		};
		
		std::sort(m_Scene.begin(), m_Scene.end(), sort);
		std::for_each(m_Scene.end() - m_DeleteBuffer.buffer.size(), m_Scene.end(), [](GameObject* go){ delete go; });
		m_Scene.erase(m_Scene.end() - m_DeleteBuffer.buffer.size(), m_Scene.end());

		m_DeleteBuffer.Reset();
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