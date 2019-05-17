#include "pch.h"
#include "Projectile.h"

Projectile::Projectile(const Vector2f& position, const Vector2f& scale, float rotation, const Vector2f& direction, GameObject* shooter)
: GameObject(position, scale, rotation)
, m_Direction(direction)
, m_Speed(1000.f)
, m_BounceCount(0)
, m_MaxBounceCount(1)
, m_pShooter(shooter)
, m_Dammage(10)
{
	m_MaxAcceleration = 1000.f;
	m_DoesCollision = false;
	m_Friction = 1.f;
}

void Projectile::Update(float dt) 
{
	if(m_ShouldDelete) return;
	
	if(m_BounceCount >= m_MaxBounceCount)
		m_pScene->Delete(this);
	
	m_Accelleration = m_Direction * m_Speed;
	m_Rotation = (std::atan2(m_Direction.y, m_Direction.x) * 180 / PI) - 90;
	GameObject::Update(dt);
}

void Projectile::Draw() const 
{
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, m_ZLayer);
	glRotatef(m_Rotation, 0.f, 0.f, 1.f);
	glScalef(m_Scale.x, m_Scale.y, 1.f);
	
	glColor4f(1.f, 1.f, 0.f, 1.f);	
	utils::FillEllipse(0.f, 0.f, 2.f, 2.f);
	
	GameObject::Draw();
	glPopMatrix();
}

void Projectile::Collision(float dt) 
{
	Point2f position = m_Position.ToPoint2f();
	Point2f direction = (m_Direction * 20.f).ToPoint2f();
	Point2f posDir = direction + position;
	
	utils::HitInfo hit;
	for(std::vector<Point2f> collider : m_pScene->GetSceneCollider())
	{
		if(utils::Raycast(collider, posDir, position, hit))
		{
			m_Direction = m_Direction.Reflect(hit.normal);
			m_Accelleration = Vector2f {};
			m_BounceCount++;
		}
	}
	
	auto scene = m_pScene->GetGameObjects();
	for(GameObject* pGo : scene)
	{
		if(utils::Raycast(pGo->GetCollider(), posDir, position, hit))
		{
			if(pGo != this && pGo != m_pShooter)
			{
				pGo->SendMessage(MessageType::dammage, m_Dammage);
				pGo->ApplyForce(m_Direction * 1000.f);
				m_pScene->Delete(this);
				break;
			}
		}
	}
}