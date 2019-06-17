#include "pch.h"
#include "AmmoPickup.h"
#include "HitMarker.h"


AmmoPickup::AmmoPickup(const Vector2f& position, const Vector2f& scale, float rotation, int ammoGain)
: PickUp(position, scale, rotation)
, m_AmmoGain(ammoGain)
, m_pTexture(ResourceManager::Get()->GetTexture("ammo"))
{
	m_ZLayer = -8;
}

void AmmoPickup::Draw() const
{
	glColor4f(1.f, 0.f, 0.f, 1.f);
	
	glPushMatrix();
	
	glTranslatef(m_Position.x, m_Position.y, m_ZLayer);
	glScalef(m_Scale.x, m_Scale.y, 1.f);
	glRotatef(m_Rotation, 0.f, 0.f, 1.f);
	
	m_pTexture->Draw(Point2f{ -(m_pTexture->GetWidth() / 2), -(m_pTexture->GetHeight() / 2) });
	
	GameObject::Draw();
	glPopMatrix();
}

int AmmoPickup::GetAmmoGain()
{
	HitMarker* pHm = new HitMarker(m_Position, "+" + std::to_string(m_AmmoGain) + " ammo!");
	m_pScene->Add(pHm);
	
	m_pScene->Delete(this); // Marks for deletion
	return m_AmmoGain; 
}

AmmoPickup::~AmmoPickup()
{
}
