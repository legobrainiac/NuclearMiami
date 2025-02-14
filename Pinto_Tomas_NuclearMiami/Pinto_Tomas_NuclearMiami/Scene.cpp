#include "pch.h"
#include "Scene.h"

#include "GameObject.h"
#include "SVGParser.h"
#include "Player.h"
#include "AiAgent.h"
#include "PickUp.h"
#include "Weapon.h"

#include "Ui/TUiManager.h"

Scene* Scene::m_psScene = nullptr;
int Scene::m_sLevel = 1;

Scene* Scene::Get()
{
	if(!m_psScene)
		m_psScene = new Scene(m_sLevel);
	
	return m_psScene;
}

Scene::Scene(int level)
: m_pScene()
, m_PlayerSpawn()
, m_AddBuffer()
{
	m_SceneMap.level = level;
	m_SceneMap.mapTextureLocation = "Resources/Scenes/Scene_" + std::to_string(level) + "/scene.png";
	m_SceneMap.colliderPointSVGLocation = "Resources/Scenes/Scene_" + std::to_string(level) + "/scene.svg";
	
	m_SceneMap.pMapTexture = ResourceManager::Get()->GetTexture("scene_" + std::to_string(level));
	
	if(SVGParser::GetVerticesFromSvgFile(m_SceneMap.colliderPointSVGLocation, m_SceneMap.sceneCollider))
		LOG("Loaded collider for scene at " << m_SceneMap.colliderPointSVGLocation); 
}

void Scene::Initialize()
{
	TUiManager::Get()->LoadUiDescriptor("Resources/Scenes/Scene_" + std::to_string(m_SceneMap.level) + "/scene.ts");
}

void Scene::Reset()
{
	delete m_psScene;
	m_psScene = nullptr;
	m_sLevel = 1;
	Scene::Get()->Initialize();
}

// Load new scene and carry the player
void Scene::Load(int scene, Player* pCarry)
{
	m_ShouldLoad = true;
	m_NextLevel = scene;
	m_pCarry = pCarry;
}

void Scene::ProcessLoad()
{
	if(!m_ShouldLoad) return;
	
	m_sLevel = m_NextLevel;
	Player* pCarry = m_pCarry;
	
	// Force removal of carry to keep it from getting deleted
	if(pCarry)
	{
		Remove(pCarry);
		ProcessRemovals();
	}
	
	// Clean reset without initializing
	delete m_psScene;
	m_psScene = nullptr;
	
	// Add carry to the scene
	Scene::Get()->Add(pCarry);
	Scene::Get()->SetPlayer(pCarry);
	pCarry->SendMessage(MessageType::regen, 20);
	
	Initialize();
	LDEBUG("----------------------------");
	LDEBUG("SCENE " + std::to_string(m_sLevel) + " loaded!!");
	LDEBUG("----------------------------");
}

void Scene::Draw() const
{	
	glPushMatrix();
	glScalef(1.f, 1.f, 0.f);
	glTranslatef(0.f, 0.f, 0.f);

	m_SceneMap.pMapTexture->Draw(Point2f {0.f, 0.f}, Rectf {0.f, 0.f, m_SceneMap.pMapTexture->GetWidth(),  m_SceneMap.pMapTexture->GetHeight()});
	
	DrawBlood();
	
	for(GameObject* pGo : m_pScene)
		pGo->Draw();
	
	glPopMatrix();
}

void Scene::Update(float dt)
{
	for(GameObject* pGo : m_pScene)
		pGo->Update(dt);
	
	// After updating, 
	ProcessRemovals();
	ProcessAdditions();
	ProcessDeletions();
	ProcessLoad();
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
		glScalef(1.f, 1.f, 1.f);
		
		utils::FillEllipse(0.f, 0.f, 3.f, 3.f);
		
		glPopMatrix();
	}
}

Scene::~Scene()
{
	for(GameObject* pGGo : m_pScene)
		delete pGGo;
}

void Scene::ProcessAdditions()
{
	if(m_AddBuffer.dirty)
	{
		for(GameObject* pGo : m_AddBuffer.buffer)
		{
			m_pScene.push_back(pGo);
			LDEBUG("GameObject( " + std::string(typeid(*pGo).name()) + " ) added... " + std::to_string(m_pScene.size()));
		}
		
		auto sort = [](GameObject* pLeft, GameObject* pRight) 
		{
			return pLeft->GetZLayer() < pRight->GetZLayer();
		};
		
		std::sort(m_pScene.begin(), m_pScene.end(), sort);
		
		m_AddBuffer.Reset();
	}
}

void Scene::ForceSort()
{
	LDEBUG("-------------- SORT --------------");
	auto sort = [](GameObject* pLeft, GameObject* pRight) 
	{
		return pLeft->GetZLayer() < pRight->GetZLayer();
	};
	
	std::sort(m_pScene.begin(), m_pScene.end(), sort);
}

void Scene::ProcessDeletions()
{
	if(m_DeleteBuffer.dirty)
	{
		// This causes very rare wrong deletes :/
		/*
		size_t objectCount = m_DeleteBuffer.buffer.size();
		
		auto sort = [](GameObject* left, GameObject* right) 
		{
			return left->IsDirty() < right->IsDirty();
		};
		
		std::sort(m_Scene.begin(), m_Scene.end(), sort);
		std::for_each(m_Scene.end() - m_DeleteBuffer.buffer.size(), m_Scene.end(), [](GameObject* go){ delete go; });
		m_Scene.erase(m_Scene.end() - m_DeleteBuffer.buffer.size(), m_Scene.end());
		*/
		
		for(GameObject* pGo : m_DeleteBuffer.buffer)
		{
			for(size_t i = 0; i < m_pScene.size(); ++i)
			{
				if(pGo == m_pScene[i])
				{
					LDEBUG("GameObject( " + std::string(typeid(*pGo).name()) + " ) Deleted... " + std::to_string(m_pScene.size()));
					
					GameObject* last = m_pScene.back();
					m_pScene[m_pScene.size() - 1] = m_pScene[i];
					m_pScene[i] = last;
					
					delete pGo;
					m_pScene.pop_back();	
				}
			}
		}
		
		m_DeleteBuffer.Reset();
	}
}

void Scene::ProcessRemovals()
{
	if(m_RemoveBuffer.dirty)
	{
		for(GameObject* pGo : m_RemoveBuffer.buffer)
		{
			for(size_t i = 0; i < m_pScene.size(); ++i)
			{
				if(pGo == m_pScene[i])
				{
					GameObject* pLast = m_pScene.back();
					m_pScene[m_pScene.size() - 1] = m_pScene[i];
					m_pScene[i] = pLast;
					m_pScene.pop_back();
					
					LDEBUG("GameObject( " + std::string(typeid(*pGo).name()) + " ) Removed... " + std::to_string(m_pScene.size()));
				}
			}
		}
		
		m_RemoveBuffer.Reset();
	}
}
