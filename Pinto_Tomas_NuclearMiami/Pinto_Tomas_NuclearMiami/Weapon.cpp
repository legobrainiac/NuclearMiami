#include "pch.h"
#include "Weapon.h"

#include "Projectile.h"
#include "Player.h"
#include "Rocket.h"

Weapon::Weapon(const Vector2f& position, const Vector2f& scale, float rotation, const std::string& texture, float kickBack, int fireRate)
: PickUp(position, scale, rotation)
, m_pTexture(ResourceManager::Get()->GetTexture(texture))
, m_KickBack(kickBack)
, m_RateOfFire(fireRate)
, m_pShootingSound(ResourceManager::Get()->GetSoundEffect("shot_basic"))
, m_pEquip(ResourceManager::Get()->GetSoundEffect("cock"))
, m_pDrop(ResourceManager::Get()->GetSoundEffect("drop"))
, m_AmmoCount(100)
{
	m_Friction = 20.f;
	m_pShootingSound->SetVolume(48);
	m_pEquip->SetVolume(64);
	m_pDrop->SetVolume(64);
	m_CircleCollider.radius = 10.f;
}

void Weapon::Draw() const
{
	glColor4f(1.f, 0.f, 0.f, 1.f);
	
	glPushMatrix();
		
	if(m_InWorld)
	{
		glTranslatef(m_Position.x, m_Position.y, m_ZLayer);
		glScalef(m_Scale.x, m_Scale.y, 1.f);
		glRotatef(m_Rotation, 0.f, 0.f, 1.f);
	}
	
	m_pTexture->Draw(Point2f{ 0.f, 0.f });
	
	GameObject::Draw();
	glPopMatrix();
}

void Weapon::Update(float dt)
{
	m_Timer += dt;
	
	if(m_InWorld)
	{
		m_Scale.x = 0.9f + std::abs(std::sin(m_Timer) * 0.1f);
		m_Scale.y = m_Scale.x;
	}
	else
	{
		m_Scale = Vector2f { 1.f, 1.f };
		
		Point2f playerPos = m_pScene->GetPlayer()->GetPosition().ToPoint2f();
		Point2f myPos = m_pOwner->GetPosition().ToPoint2f();
		
		Vector2f dir { playerPos, myPos };
		m_RotationToPlayer = std::atan2(dir.y, dir.x)  * (180.f / PI) - 90.f;
		m_DistanceToPlayer = dir.Length() / 4.f;
	}
	PickUp::Update(dt);
}

void Weapon::SetInWorld(bool val)
{
	PickUp::SetInWorld(val);

	if(val)
		m_pDrop->PlayDirectional(m_RotationToPlayer, m_DistanceToPlayer);
	else
		m_pEquip->PlayDirectional(m_RotationToPlayer, m_DistanceToPlayer);
}

void Weapon::Shoot(const Vector2f& position, const Vector2f& direction, float rofMod)
{
	if(m_Timer < (1.f / (m_RateOfFire / rofMod)) || m_InWorld || m_AmmoCount <= 0) return;	
	
	m_Timer = 0.f;
	Projectile* projectile = new Projectile(position, Vector2f {1.f, 1.f}, 0.f, direction.Normalized(), m_pOwner);
	m_pScene->Add(projectile);
	
	Vector2f kickBack = Vector2f{direction.Normalized().ToPoint2f(), Point2f{ 0.f, 0.f }};
	
	kickBack *= m_KickBack;
	m_pOwner->ApplyForce(kickBack);
	
	if(rofMod > 1)
		m_pShootingSound->PlayDirectional(m_RotationToPlayer, m_DistanceToPlayer);		
	else
		m_pShootingSound->Play(0);
	
	m_AmmoCount--;
}

Weapon::~Weapon()
{
	
}
