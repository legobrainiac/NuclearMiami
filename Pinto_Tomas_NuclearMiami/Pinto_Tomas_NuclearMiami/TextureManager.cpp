#include "pch.h"
#include "TextureManager.h"
#include "Texture.h"

TextureManager* TextureManager::m_pTextureManager = nullptr;

TextureManager* TextureManager::Get()
{
	if(!m_pTextureManager)
		m_pTextureManager = new TextureManager();
	
	return m_pTextureManager;
}

Texture* TextureManager::GetTexture(const std::string& name)
{
	for(const std::pair<std::string, Texture*>& tex : m_TextureMap)
	{
		if(tex.first == name)
			return tex.second;
	}

	ERR("Texture '" << name << "' should be preloaded...");
	return nullptr;
}

Texture* TextureManager::LoadTexture(const std::string& path, const std::string& name)
{
	for(std::pair<std::string, Texture*> tex : m_TextureMap)
	{
		if(tex.first == name)
		{
			ERR("Texture '" << name << "' already loaded...");
			return m_TextureMap[name];
		}
	}
	
	m_TextureMap[name] = new Texture(path);
	return m_TextureMap[name];
}

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
	for(std::pair<std::string, Texture*> tex : m_TextureMap)
	{
		delete tex.second;
	}
}
