#ifndef TEXTUREOBJECT_H
#define TEXTUREOBJECT_H

#include "GameObject.h"

class Texture;

class TextureObject :
	public GameObject
{
public:
	TextureObject(const Vector2f& pos, float rotation, const std::string& texture);
	void Draw() const;
	
private:
	Texture* m_pTexture;
};

#endif // !TEXTUREOBJECT_H