#include "pch.h"
#include "NextLevelPad.h"

#include "Texture.h"
#include "Player.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "AiAgent.h"

NextLevelPad::NextLevelPad(const Vector2f& position, int level, float range)
: GameObject(position, Vector2f { 0.f, 0.f }, 0)
, m_Level(level)
, m_Range(range)
, m_pTexture(ResourceManager::Get()->GetTexture("nextLevelPad"))
{
	m_DoesCollision = false;
	m_ZLayer = -5;
}

NextLevelPad::~NextLevelPad()
{
}

void NextLevelPad::Draw() const 
{
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, m_ZLayer);
	m_pTexture->Draw();
	glPopMatrix();
}

void NextLevelPad::Update(float dt) 
{
	Player* pPlayer = Scene::Get()->GetPlayer();
	
	if(pPlayer)
	{
		if(pPlayer->GetPosition().ToPoint2f().DistanceTo(m_Position.ToPoint2f()) < m_Range && AiAgent::GetAiInstanceCount() <= 0) 
		{
			Scene::Get()->Load(m_Level, pPlayer);
		}
	}
}