#ifndef TEXT_RENDER_H
#define TEXT_RENDER_H

#include <string>

class Texture;

struct TextRenderConfig
{
	float spacing;
	float scale;
};

class TextRenderer
{
public:
	TextRenderer(const std::string& path, const Color4f color);
	
	TextRenderer(const TextRenderer& other) = delete;
	TextRenderer& operator=(const TextRenderer& other) = delete;
	TextRenderer(TextRenderer&&) = delete;
	TextRenderer& operator=(TextRenderer&&) = delete;
	
	~TextRenderer();
	
	void DrawString(const std::string& str, const Vector2f& pos, const TextRenderConfig& cfg) const;
	Texture* GetChar(char c) const;
	
private:
	TTF_Font* m_pFont;
	Texture* m_pCharTex[127];
	Color4f m_Color;
};

#endif // !TEXT_RENDER_H

