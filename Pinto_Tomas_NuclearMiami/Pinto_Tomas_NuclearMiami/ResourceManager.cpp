#include "pch.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "SoundEffect.h"

ResourceManager* ResourceManager::m_pTextureManager = nullptr;

ResourceManager* ResourceManager::Get()
{
	if(!m_pTextureManager)
		m_pTextureManager = new ResourceManager();
	
	return m_pTextureManager;
}

Texture* ResourceManager::GetTexture(const std::string& name)
{
	for(const std::pair<std::string, Texture*>& tex : m_TextureMap)
	{
		if(tex.first == name)
			return tex.second;
	}

	ERR("Texture '" << name << "' should be preloaded...");
	return nullptr;
}

Texture* ResourceManager::LoadTexture(const std::string& path, const std::string& name)
{
	for(std::pair<std::string, Texture*> tex : m_TextureMap)
	{
		if(tex.first == name)
		{
			ERR("Texture '" << name << "' already loaded...");
			return m_TextureMap[name];
		}
	}

	LOG("Preloading Texture: " << name);
	m_TextureMap[name] = new Texture(path);
	return m_TextureMap[name];
}

SoundEffect* ResourceManager::GetSoundEffect(const std::string& name)
{
	for(const std::pair<std::string, SoundEffect*>& snd : m_SoundEffectMap)
	{
		if(snd.first == name)
			return snd.second;
	}

	ERR("Texture '" << name << "' should be preloaded...");
	return nullptr;
}

SoundEffect* ResourceManager::LoadSoundEffect(const std::string& path, const std::string& name)
{
	for(std::pair<std::string, SoundEffect*> snd : m_SoundEffectMap)
	{
		if(snd.first == name)
		{
			ERR("SoundEffect '" << name << "' already loaded...");
			return m_SoundEffectMap[name];
		}
	}

	LOG("Preloading SoundEffect: " << name);
	m_SoundEffectMap[name] = new SoundEffect(path);
	return m_SoundEffectMap[name];
}

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
	for(std::pair<std::string, Texture*> tex : m_TextureMap)
		delete tex.second;
	
	for(std::pair<std::string, SoundEffect*> snd : m_SoundEffectMap)
		delete snd.second;
}
