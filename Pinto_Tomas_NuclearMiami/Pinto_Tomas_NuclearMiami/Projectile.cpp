#include "pch.h"
#include "Projectile.h"


Projectile::Projectile(const Vector2f& position, const Vector2f& scale, float rotation, const Vector2f& direction)
: GameObject(position, scale, rotation)
, m_Direction(direction)
, m_Speed(1000.f)
{
}

void Projectile::Update(float dt) 
{
	Translate(m_Direction * m_Speed * dt);
}

void Projectile::Draw() const 
{
	glPushMatrix();
	
	glTranslatef(m_Position.x, m_Position.y, m_ZLayer);
	glRotatef(m_Rotation, 0.f, 0.f, 1.f);
	glScalef(m_Scale.x, m_Scale.y, 0.f);
	
	glColor4f(1.f, 0.f, 0.f, 1.f);	
	utils::DrawPoint(0.f, 0.f, 10.f);
	
	GameObject::Draw();
	
	glPopMatrix();
}