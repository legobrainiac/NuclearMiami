#include "pch.h"

#include "Scene.h"
#include "Texture.h"
#include "GameObject.h"
#include "SVGParser.h"
#include "Player.h"
#include "AiAgent.h"

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
	
	auto aiAgentTest = new AiAgent(Vector2f { 200.f, 300.f }, Vector2f { 1.f, 1.f }, 0.f, m_pPlayer, this);
	aiAgentTest->SetZLayer(-1.f);

	auto aiAgentTest2 = new AiAgent(Vector2f { 600.f, 100.f }, Vector2f { 1.f, 1.f }, 0.f, m_pPlayer, this);
	aiAgentTest2->SetZLayer(-1.f);
	
	Add(m_pPlayer);
	Add(aiAgentTest);
	Add(aiAgentTest2);
}

void Scene::Draw() const
{	
	glPushMatrix();
	glScalef(1.f, 1.f, 0.f);
	glTranslatef(0.f, 0.f, 0.f);

	m_SceneMap.pMapTexture->Draw(Point2f {0.f, 0.f}, Rectf {0.f, 0.f, m_SceneMap.pMapTexture->GetWidth(),  m_SceneMap.pMapTexture->GetHeight()});
	
	//utils::DrawPolygon(m_SceneMap.sceneCollider[0], true, 2.f);
	
	for(auto go : m_Scene)
		go->Draw();
	
	glPopMatrix();
}

void Scene::Update(float dt)
{
	for(auto go : m_Scene)
		go->Update(dt);
	
	// After updating, 
	ProcessAdditions();
	ProcessDeletions();
}

void Scene::Add(GameObject* pGameObject)
{
	if(pGameObject != nullptr)
		m_AddBuffer.Add(pGameObject);
}

void Scene::Delete(GameObject* pGameObject)
{
	if(pGameObject != nullptr)
	{
		pGameObject->MakeDirty();
		m_DeleteBuffer.Add(pGameObject);
	}
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
		for(auto go : m_AddBuffer.buffer)
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