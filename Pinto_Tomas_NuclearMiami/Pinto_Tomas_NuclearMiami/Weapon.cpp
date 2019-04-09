#include "pch.h"
#include "Weapon.h"
#include "Player.h"
#include "Texture.h"
#include "Projectile.h"
#include "Scene.h"

Weapon::Weapon(const Vector2f& position, const Vector2f& scale, float rotation, GameObject* pOwner, Scene* pScene, const std::string& texturePath, float kickBack)
: PickUp(position, scale, rotation, pOwner, pScene)
, m_pTexture(new Texture(texturePath))
, m_KickBack(kickBack)
, m_RateOfFire(10)
{
	m_Friction = 20.f;
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
}

Weapon::~Weapon()
{
	delete m_pTexture;
}
