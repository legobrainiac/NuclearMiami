#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <string>
#include <map>

#include "utils.h"

class Texture;

class TextureManager
{
public:
	static TextureManager* Get();
	
	Texture* GetTexture(const std::string& name);
	Texture* LoadTexture(const std::string& path, const std::string& name);
	~TextureManager();

private:
	TextureManager();
	
	std::map<std::string, Texture*> m_TextureMap;
	static TextureManager* m_pTextureManager;
};

#endif // !TEXTURE_MANAGER_H