#include "pch.h"
#include "Weapon.h"
#include "Player.h"

Weapon::Weapon(const Vector2f& position, const Vector2f& scale, float rotation, Player* player)
: PickUp(position, scale, rotation, player)
{
}

void Weapon::Draw() const
{
	glColor4f(1.f, 0.f, 0.f, 1.f);
	
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, m_ZLayer);
	glRotatef(m_Rotation, 0.f, 0.f, 1.f);
	glScalef(m_Scale.x, m_Scale.y, 0.f);
	
	utils::FillRect(-5.f, -5.f, 10.f, 10.f);
	
	GameObject::Draw();
	glPopMatrix();
}

Weapon::~Weapon()
{
}
