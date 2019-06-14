#include "pch.h"
#include "TextRenderer.h"
#include "Texture.h"
#include "utils.h"
#include "structs.h"

TextRenderer::TextRenderer(const std::string& path, const Color4f color)
: m_pFont(TTF_OpenFont(path.c_str(), 30))
, m_Color(color)
{
	if (m_pFont == nullptr)
	{
		ERR("Failed to load font" << TTF_GetError());
		return;
	}
	
	for(char c = 0; c < 127; c++)
	{
		std::string s(1, c);
		m_pCharTex[int(c)] = new Texture(s, m_pFont, color);
	}
	
	TTF_CloseFont(m_pFont);
}

void TextRenderer::DrawString(const std::string& str, const Vector2f& pos, const TextRenderConfig& cfg) const
{
	glPushMatrix();
	glScalef(cfg.scale, cfg.scale, 0.f);
	glTranslatef(pos.x, pos.y, 0.f);
	
	float totalX = 0;
	
	for(size_t i = 0; i < str.size(); ++i)
	{
		Texture* pCharTex = m_pCharTex[int(str[i])];
		
		if(pCharTex)
		{
			float posX = totalX + cfg.spacing;
			totalX += pCharTex->GetWidth();			

			pCharTex->Draw(Point2f { posX, 0 });
		}
	}
	
	glPopMatrix();
}

Texture* TextRenderer::GetChar(char c) const
{
	return m_pCharTex[int(c)];
}

TextRenderer::~TextRenderer()
{
	for(char c = 0; c < 127; c++)
		delete m_pCharTex[int(c)];
}
