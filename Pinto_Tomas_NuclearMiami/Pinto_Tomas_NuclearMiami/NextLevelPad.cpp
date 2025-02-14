#include "pch.h"
#include "NextLevelPad.h"

#include "AiAgent.h"
#include "Player.h"

int NextLevelPad::m_pPadCount = 0;

NextLevelPad::NextLevelPad(const Vector2f& position, int level, float range)
: GameObject(position, Vector2f { 0.f, 0.f }, 0)
, m_Level(level)
, m_Range(range)
, m_pTexture(ResourceManager::Get()->GetTexture("nextLevelPad"))
{
	m_DoesCollision = false;
	m_ZLayer = -5;
	m_pPadCount++;
}

NextLevelPad::~NextLevelPad()
{
	m_pPadCount--;
}

void NextLevelPad::Draw() const 
{
	TextRenderConfig textConfig;
	textConfig.spacing = 3.f;
	textConfig.scale = 1.f;
	
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, m_ZLayer);
	m_pTexture->Draw(Point2f{ -(m_pTexture->GetWidth() / 2), -(m_pTexture->GetHeight() / 2) });

	// Text
	glPushMatrix();
	glScalef(0.25f, 0.25f, 1.f);
	ResourceManager::Get()->GetTextRenderer("munro")->DrawString("NEXT LEVEL", Vector2f { -70.f, 70.f }, textConfig);
	
	glPopMatrix();
	glPopMatrix();	
}

void NextLevelPad::Update(float dt) 
{
	Player* pPlayer = Scene::Get()->GetPlayer();
	
	if(pPlayer)
		if(pPlayer->GetPosition().ToPoint2f().DistanceTo(m_Position.ToPoint2f()) < m_Range && AiAgent::GetAiInstanceCount() <= 0) 
			Scene::Get()->Load(m_Level, pPlayer);
}