#include "pch.h"
#include "Rocket.h"

#include "Explosion.h"

Rocket::Rocket(const Vector2f& position, const Vector2f& scale, float rotation, const Vector2f& direction, GameObject* shooter)
: Projectile(position, scale, rotation, direction, shooter)
, m_pTexture(ResourceManager::Get()->GetTexture("rocket"))
, m_Radius(100)
{
	m_Speed = 500;
	m_BounceCount = 0;
	m_MaxBounceCount = 1;
	m_Dammage = 80;
}

void Rocket::Draw() const
{
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, m_ZLayer);
	glScalef(m_Scale.x, m_Scale.y, 1.f);
	glRotatef(m_Rotation, 0.f, 0.f, 1.f);
	glColor4f(1.f, 1.f, 0.f, 1.f);	

	m_pTexture->Draw(Point2f{ -(m_pTexture->GetWidth() / 2), -(m_pTexture->GetHeight() / 2) });
	
	GameObject::Draw();
	glPopMatrix();
}

void Rocket::DoHit()
{
	Explosion* pExplosion = new Explosion(m_Position);
	m_pScene->Add(pExplosion);
	
	for(GameObject* go : m_pScene->GetGameObjects())
	{
		if(go != m_pShooter)
		{
			Point2f pos = m_Position.ToPoint2f();
			Point2f otherPos = go->GetPosition().ToPoint2f();
			
			Vector2f dir { pos, otherPos };
			
			if(pos.DistanceTo(otherPos) < m_Radius && InSight(m_Position.ToPoint2f(), go->GetPosition().ToPoint2f()))
			{
				go->SendMessage(MessageType::dammage, m_Dammage);
				go->ApplyForce(dir.Normalized() * 1000.f); // TODO(tomas): change this to correct direction
			}
		}
	}
}


bool Rocket::InSight(Point2f tail, Point2f head)
{
	utils::HitInfo hit;
	
	for (std::vector<Point2f> collider : m_pScene->GetSceneCollider())
	{
		if (Raycast(collider, tail, head, hit))
			return false;
	}
	
	return true;
}

void Rocket::Collision(float dt) 
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
			
			if(m_BounceCount >= m_MaxBounceCount)
			{
				DoHit();
				m_pScene->Delete(this);
			}
		}
	}
	
	auto scene = m_pScene->GetGameObjects();
	for(GameObject* go : scene)
	{
		if(utils::Raycast(go->GetCollider(), posDir, position, hit))
		{
			if(go != this && go != m_pShooter)
			{
				DoHit();
				m_pScene->Delete(this);
				break;
			}
		}
	}
}

Rocket::~Rocket()
{
}
