#include "pch.h"
#include "BossFight.h"
#include "Player.h"

int BossFight::m_BossFightCounter = 0;

BossFight::BossFight(const Vector2f& position, const Vector2f& scale, float rotation)
: GameObject(position, scale, rotation)
, m_pBossTexture(ResourceManager::Get()->GetTexture("bossMain"))
, m_pArmTexture(ResourceManager::Get()->GetTexture("bossArm"))
{
	m_CircleCollider.radius = 55.f;
	m_BossFightCounter++;
	
	m_VertexCollider.push_back(Point2f { -50.f, -50.f });
	m_VertexCollider.push_back(Point2f { -50.f, 50.f });
	m_VertexCollider.push_back(Point2f { 50.f, 50.f });
	m_VertexCollider.push_back(Point2f { 50.f, -50.f });
}

void BossFight::Draw() const
{
	// Draw Potatoe
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, 0.f);
	glScalef(0.5f, 0.5f, 1.f);

	m_pBossTexture->Draw(Point2f{ -(m_pBossTexture->GetWidth() / 2), -(m_pBossTexture->GetHeight() / 2) });

	// Draw arm
	glPushMatrix();
	glRotatef(m_ArmRotation, 0.f, 0.f, 1.f);
	glTranslatef(90.f, 0.f, 0.f);
	
	m_pArmTexture->Draw(Point2f{ 0.f, -(m_pArmTexture->GetHeight() / 2) });
	
	glPopMatrix();
	
	GameObject::Draw();
	glPopMatrix();		
}

void BossFight::Update(float dt)
{
	Player* pTarget = m_pScene->GetPlayer();
	
	Vector2f targetPos = pTarget->GetPosition();
	Vector2f direction = Vector2f { m_Position.ToPoint2f(), targetPos.ToPoint2f() }.Normalized();
	
	float dirRot = atan2(direction.y, direction.x) * 180.f / PI;
	
	//Vector2f armDiretion { cos(m_ArmRotation * PI / 180.f), sin(m_ArmRotation * PI / 180.f) };
	//armDiretion *= 50.f;
	
	m_ArmRotation = utils::LerpDegrees(m_ArmRotation, dirRot, dt * 6.f);
	
	GameObject::Update(dt);
}

BossFight::~BossFight()
{
	m_BossFightCounter--;
}
