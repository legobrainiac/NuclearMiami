#include "pch.h"
#include "TextureObject.h"

TextureObject::TextureObject(const Vector2f& pos, float rotation, const std::string& texture)
: GameObject(pos, Vector2f { 1.f, 1.f }, rotation)
, m_pTexture(ResourceManager::Get()->GetTexture(texture))
{
	m_DoesCollision = false; 
	m_ZLayer = -10;
}

void TextureObject::Draw() const
{
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, 0.f);	
	glScalef(1.5f, 1.5f, 1.f);
	glRotatef(m_Rotation, 0.f, 0.f, 1.f);
	m_pTexture->Draw(Point2f{-(m_pTexture->GetWidth() / 2.f), -(m_pTexture->GetHeight() / 2.f)});
	glPopMatrix();
}