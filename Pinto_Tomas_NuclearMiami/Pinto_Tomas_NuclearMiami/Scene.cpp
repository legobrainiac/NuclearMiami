#include "pch.h"

#include "Scene.h"
#include "Texture.h"
#include "GameObject.h"
#include "SVGParser.h"
#include "Player.h"
#include "AiAgent.h"
#include "PickUp.h"
#include "Weapon.h"

#include "Ui/TUiManager.h"

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
	TUiManager::Get()->LoadUiDescriptor("Resources/Scenes/Scene3/scene3.ts");
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

void Scene::ForceSort()
{
	auto sort = [](GameObject* left, GameObject* right) 
	{
		return left->GetZLayer() < right->GetZLayer();
	};
	
	std::sort(m_Scene.begin(), m_Scene.end(), sort);
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
