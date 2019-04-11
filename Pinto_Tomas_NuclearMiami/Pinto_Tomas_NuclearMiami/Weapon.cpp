#include "pch.h"
#include "Weapon.h"
#include "Player.h"
#include "Texture.h"
#include "Projectile.h"
#include "Scene.h"
#include "SoundEffect.h"
#include "ResourceManager.h"

Weapon::Weapon(const Vector2f& position, const Vector2f& scale, float rotation, GameObject* pOwner, Scene* pScene, const std::string& texture, float kickBack, int fireRate)
: PickUp(position, scale, rotation, pOwner, pScene)
, m_pTexture(ResourceManager::Get()->GetTexture(texture))
, m_KickBack(kickBack)
, m_RateOfFire(fireRate)
, m_pShootingSound(ResourceManager::Get()->GetSoundEffect("shot_basic"))
, m_pEquip(ResourceManager::Get()->GetSoundEffect("cock"))
, m_pDrop(ResourceManager::Get()->GetSoundEffect("drop"))
{
	m_Friction = 20.f;
	m_pShootingSound->SetVolume(64);
	m_pEquip->SetVolume(64);
	m_pDrop->SetVolume(64);
}

void Weapon::Draw() const
{
	glColor4f(1.f, 0.f, 0.f, 1.f);
	
	glPushMatrix();
		
	if(m_InWorld)
	{
		glTranslatef(m_Position.x, m_Position.y, m_ZLayer);
		glRotatef(m_Rotation, 0.f, 0.f, 1.f);
	}
	
	m_pTexture->Draw(Point2f{ 0.f, 0.f });
	
	GameObject::Draw();
	glPopMatrix();
}

void Weapon::Update(float dt)
{
	m_Timer += dt;
	PickUp::Update(dt);
}

void Weapon::SetInWorld(bool val)
{
	PickUp::SetInWorld(val);

	if(val)
		m_pDrop->Play(0);
	else
		m_pEquip->Play(0);
}

// TODO(tomas): make this virtual
void Weapon::Shoot(const Vector2f& position, const Vector2f& direction, Scene* pScene)
{
	if(m_Timer < (1.f / m_RateOfFire) || m_InWorld) return;	
	
	m_Timer = 0.f;
	Projectile* projectile = new Projectile(position, Vector2f {1.f, 1.f}, 0.f, direction.Normalized(), pScene, m_pOwner);
	pScene->Add(projectile);
	
	Vector2f kickBack = Vector2f{direction.Normalized().ToPoint2f(), Point2f{ 0.f, 0.f }};
	
	kickBack *= m_KickBack;
	
	m_pOwner->ApplyForce(kickBack);
	
	m_pShootingSound->Play(0);
}

Weapon::~Weapon()
{
}
