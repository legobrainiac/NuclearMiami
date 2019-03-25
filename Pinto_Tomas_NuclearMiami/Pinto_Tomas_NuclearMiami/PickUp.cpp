#include "pch.h"
#include "PickUp.h"
#include "utils.h"
#include "Player.h"

PickUp::PickUp(const Vector2f& position, const Vector2f& scale, float rotation, Player* player)
: GameObject(position, scale, rotation)
, m_pPlayer(player)
{
}

PickUp::~PickUp()
{
}

void PickUp::Update(float dt)
{
	if(m_pPlayer && m_pPlayer->HasEmptySlot())
	{
		Point2f pos = m_Position.ToPoint2f();
		Point2f playerPos = m_pPlayer->GetPosition().ToPoint2f();
		Vector2f direction { pos, playerPos };
		
		if(direction.Length() < 100)
		{
			ApplyForce(direction.Normalized() * 10.f);
			
			if(direction.Length() < 10)
				m_pPlayer->ProcessPickUp(this);
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