#include "pch.h"
#include "Sprite.h"
#include "Texture.h"
#include "TextureManager.h"

#include <iostream>

Sprite::Sprite(const std::string& texName, int nrCols, int nrRows, float frameSec)
: m_pTexture{ TextureManager::Get()->GetTexture(texName) }
, m_Cols(nrCols)
, m_Rows(nrRows)
, m_FrameSec(frameSec)
{

}

Sprite::~Sprite()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void Sprite::Draw(const Point2f& pos, float scale, bool invertDirection) const
{
	Rectf destRect;
	int frameTotal = m_Cols * m_Rows;

	destRect.left = pos.x;
	destRect.bottom = pos.y;
	destRect.width = scale * GetFrameWidth();
	destRect.height = scale * GetFrameHeight();

	Rectf sourceRect;
	sourceRect.width = GetFrameWidth();
	sourceRect.height = GetFrameHeight();

	int row, col;

	if (invertDirection)
	{
		row = m_ActFrame / m_Cols;
		col = m_ActFrame % (m_Rows + 1);
	}
	else
	{
		row = m_ActFrame / m_Rows;
		col = m_ActFrame % m_Cols;
	}

	sourceRect.left = GetFrameWidth() * col;
	sourceRect.bottom = GetFrameHeight() * (row + 1);

	m_pTexture->Draw(destRect, sourceRect);
}

void Sprite::Update(float dt)
{
	int frameTotal = m_Cols * m_Rows;
	m_AccuSec += dt;

	if (m_AccuSec > m_FrameSec)
	{
		m_AccuSec -= m_AccuSec;

		if (m_ActFrame != frameTotal - 1)
			m_ActFrame++;
		else
			m_ActFrame = 0;
	}
}

float Sprite::GetFrameWidth() const
{
	return m_pTexture->GetWidth() / m_Cols;
}

float Sprite::GetFrameHeight() const
{
	return m_pTexture->GetHeight() / m_Rows;
}
