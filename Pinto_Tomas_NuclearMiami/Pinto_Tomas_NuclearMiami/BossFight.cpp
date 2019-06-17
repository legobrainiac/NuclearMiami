#include "pch.h"
#include "SoundEffect.h" 
#include "BossFight.h"
#include "Explosion.h"
#include "Player.h"
#include "Rocket.h"
#include "Scene.h"

int BossFight::m_BossFightCounter = 0;

BossFight::BossFight(const Vector2f& position, const Vector2f& scale, float rotation)
: GameObject(position, scale, rotation)
, m_pBossTexture(ResourceManager::Get()->GetTexture("bossMain"))
, m_pArmTexture(ResourceManager::Get()->GetTexture("bossArm"))
, m_Health(BOSS_HEALTH)
, m_CenterPosition(position)
, m_State(BossState::Alive)
{
	m_pShootingSound = ResourceManager::Get()->GetSoundEffect("shotgun");
	
	m_CircleCollider.radius = 55.f;
	m_BossFightCounter++;
	
	m_VertexCollider.push_back(Point2f { -50.f, -50.f });
	m_VertexCollider.push_back(Point2f { -50.f, 50.f });
	m_VertexCollider.push_back(Point2f { 50.f, 50.f });
	m_VertexCollider.push_back(Point2f { 50.f, -50.f });
}

void BossFight::Draw() const
{
	if(m_State == BossState::Dead) return;
	
	// Draw Potatoe
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, 0.f);
	glScalef(0.5f, 0.5f, 1.f);

	m_pBossTexture->Draw(Point2f{ -(m_pBossTexture->GetWidth() / 2.f), -(m_pBossTexture->GetHeight() / 2) });

	DrawHealth();
	DrawArm();
}

void BossFight::DrawHealth() const
{
	if(m_State == BossState::Dying) return;
	
	// Draw Health
	glPushMatrix();
	glTranslatef(-90.f, 100.f, 0.f);
	
	glColor4f(1.f, 0.f, 0.f, 1.f);
	utils::FillRect(0.f, 0.f, 200.f, 20.f);
	
	glColor4f(0.f, 1.f, 0.f, 1.f);
	utils::FillRect(0.f, 0.f, m_Health / 5.f, 20.f);
	
	glPopMatrix();
}

void BossFight::DrawArm() const
{
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
	if(m_State == BossState::Alive)
	{
		Player* pTarget = m_pScene->GetPlayer();
		
		Vector2f targetPos = pTarget->GetPosition();
		Vector2f direction = Vector2f { m_Position.ToPoint2f(), targetPos.ToPoint2f() }.Normalized();
		
		// Aim arm at player
		float dirRot = atan2(direction.y, direction.x) * 180.f / PI;
		m_ArmRotation = utils::LerpDegrees(m_ArmRotation, dirRot, dt * 6.f);
		
		// Move to center
		m_Position = utils::Lerp(m_Position, m_CenterPosition, dt * 5.f);
		
		Shoot(dt);
	}
	else if(m_State == BossState::Dying)
	{
		m_DyingTimer += dt;
		if(m_DyingTimer > .1f)
		{
			m_DyingTimer -= m_DyingTimer;
			
			int randomRotation = utils::RandInterval(0, 360);
			Vector2f dir { cos(randomRotation * PI / 180.f), sin(randomRotation * PI / 180.f) };
			dir *= 30.f;
			
			Explosion* pExplosion = new Explosion(m_Position + dir);
			m_pScene->Add(pExplosion);
			
			m_ExplosionCounter++;
			
			if(m_ExplosionCounter == 20) 
				m_State = BossState::Dead;
		}
	}
	else
	{
		m_pScene->Delete(this);
	}
	
	GameObject::Update(dt);
}

void BossFight::Shoot(float dt)
{
	m_ShotTimer += dt;
	
	if(m_ShotTimer > 2.5f)
	{
		m_ShotTimer -= m_ShotTimer;

		Vector2f armDiretion { cos(m_ArmRotation * PI / 180.f), sin(m_ArmRotation * PI / 180.f) };
		armDiretion *= 50.f;
		
		Rocket* projectile = new Rocket(m_Position + armDiretion, Vector2f {1.f, 1.f}, 0.f, armDiretion.Normalized(), this);
		m_pScene->Add(projectile);
		
		Vector2f kickBack = Vector2f{armDiretion.Normalized().ToPoint2f(), Point2f{ 0.f, 0.f }};
		
		kickBack *= 1000.f;
		ApplyForce(kickBack);
		
		m_pShootingSound->Play(0);		
	}
}

void BossFight::SendMessage(MessageType message, int value)
{
	if(message == MessageType::dammage && m_Health > 0)
	{
		m_Health -= value;
		
		if(m_Health <= 0)
			m_State = BossState::Dying;
	}
}

BossFight::~BossFight()
{
	m_BossFightCounter--;
}
