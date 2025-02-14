#include "pch.h"
#include "PickUp.h"

#include "Player.h"

PickUp::PickUp(const Vector2f& position, const Vector2f& scale, float rotation)
: GameObject(position, scale, rotation)
, m_InWorld(true)
{
	m_DoesCollision = false;
	
	m_VertexCollider.push_back(Point2f {-10.f, -5.f});
	m_VertexCollider.push_back(Point2f {-10.f, 5.f});
	m_VertexCollider.push_back(Point2f {10.f, 5.f});
	m_VertexCollider.push_back(Point2f {10.f, -5.f});
	
	m_ZLayer = -10;
}

void PickUp::Update(float dt)
{
	m_PickupTimer += dt;

	if(m_InWorld && m_PickupTimer > 1.f)
	{
		for(GameObject* pGo : m_pScene->GetGameObjects())
		{
			Point2f pos = m_Position.ToPoint2f();
			Point2f goPos = pGo->GetPosition().ToPoint2f();
			
			if(pos.DistanceTo(goPos + Vector2f { 5.f, 5.f }) < 20.f)		
			{
				if(pGo->ProcessPickUp(this)) 
					break;
			}
		}
	}
	
	GameObject::Update(dt);
}

void PickUp::Draw() const 
{
	glColor4f(0.f, 0.f, 1.f, 1.f);

	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, m_ZLayer);
	glRotatef(m_Rotation, 0.f, 0.f, 1.f);
	glScalef(m_Scale.x, m_Scale.y, 0.f);
	
	utils::FillRect(-5.f, -5.f, 10.f, 10.f);
	
	GameObject::Draw();
	glPopMatrix();
}

void PickUp::SetInWorld(bool val)
{
	m_InWorld = val; 
	m_PickupTimer = 0.f; 
	
	if(val)
		m_pOwner = nullptr;
	
	LDEBUG("PickUp world status changed: " + std::to_string(m_InWorld));
}
