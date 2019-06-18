#include "pch.h"
#include "HitMarker.h"

HitMarker::HitMarker(const Vector2f& position, const std::string& text)
: GameObject(position, Vector2f { 1.f, 1.f }, 0)
, m_HitText(text)
{
	m_pTextRenderer = ResourceManager::Get()->GetTextRenderer("munro");
	m_DoesCollision = false;
	m_ZLayer = 10;
}

void HitMarker::Draw() const
{
	TextRenderConfig cfg;
	cfg.spacing = 3;
	cfg.scale = 0.4f;
	
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, 0.f);
	m_pTextRenderer->DrawString(m_HitText, Vector2f { 0.f, 0.f }, cfg);
	glPopMatrix();
}
	
void HitMarker::Update(float dt)
{
	if(m_DeleteTimer > m_LifeTime)
		m_pScene->Delete(this);
	
	m_DeleteTimer += dt;
	m_Position.y += 20.f * dt;
}


