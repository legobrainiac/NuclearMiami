#include "pch.h"
#include "PickUp.h"
#include "utils.h"

// TODO(tomas): take player pointer, tell player to pick me up
PickUp::PickUp(const Vector2f& position, const Vector2f& scale, float rotation)
: GameObject(position, scale, rotation)
{
}

PickUp::~PickUp()
{
}

void PickUp::Update(float dt)
{

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