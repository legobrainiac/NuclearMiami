#ifndef TEXT_RENDER_H
#define TEXT_RENDER_H

#include <map>
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
	TextRenderer(const std::string& path);
	~TextRenderer();
	
	void DrawString(const std::string& str, const Vector2f& pos, const TextRenderConfig& cfg) const;
	Texture* GetChar(char c) const;
	
private:
	TTF_Font* m_pFont;
	std::map<char, Texture*> m_CharTex;
};

#endif // !TEXT_RENDER_H

