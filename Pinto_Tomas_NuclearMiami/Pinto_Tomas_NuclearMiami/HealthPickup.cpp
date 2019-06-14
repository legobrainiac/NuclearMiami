#include "pch.h"
#include "HealthPickup.h"
#include "HitMarker.h"

HealthPickup::HealthPickup(const Vector2f& position, const Vector2f& scale, float rotation, int healthGain)
: PickUp(position, scale, rotation)
, m_HealthGain(healthGain)
, m_pTexture(ResourceManager::Get()->GetTexture("health"))
{
	
}

void HealthPickup::Draw() const
{
	glColor4f(1.f, 0.f, 0.f, 1.f);
	
	glPushMatrix();
	
	glTranslatef(m_Position.x, m_Position.y, m_ZLayer);
	glScalef(m_Scale.x, m_Scale.y, 1.f);
	glRotatef(m_Rotation, 0.f, 0.f, 1.f);
	
	m_pTexture->Draw(Point2f{ 0.f, 0.f });
	
	GameObject::Draw();
	glPopMatrix();
}

int HealthPickup::GetHealthGain()
{ 
	HitMarker* pHm = new HitMarker(m_Position, "+" + std::to_string(m_HealthGain));
	m_pScene->Add(pHm);
	
	m_pScene->Delete(this); // Marks for deletion
	return m_HealthGain; 
}