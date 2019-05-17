#include "pch.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::m_pTextureManager = nullptr;

ResourceManager* ResourceManager::Get()
{
	if(!m_pTextureManager)
		m_pTextureManager = new ResourceManager();
	
	return m_pTextureManager;
}

// Texture pre-loading
Texture* ResourceManager::GetTexture(const std::string& name)
{
	return m_TextureMap.find(name)->second;
}

Texture* ResourceManager::LoadTexture(const std::string& path, const std::string& name)
{
	auto pTex = m_TextureMap.find(name);
	
	if(pTex != m_TextureMap.end())
	{
		ERR("Texture '" << name << "' already loaded...");
		return pTex->second;
	}
	else
	{
		LOG("Preloading Texture: " << name);
		m_TextureMap[name] = new Texture(path);
		return m_TextureMap[name];
	}
}

// SoundEffect pre-loading
SoundEffect* ResourceManager::GetSoundEffect(const std::string& name)
{
	return m_SoundEffectMap.find(name)->second;
}

SoundEffect* ResourceManager::LoadSoundEffect(const std::string& path, const std::string& name)
{
	auto pSound = m_SoundEffectMap.find(name);
	if(pSound != m_SoundEffectMap.end())
	{
		ERR("SoundEffect '" << name << "' already loaded...");
		return pSound->second;
	}
	else
	{
		LOG("Preloading SoundEffect: " << name);
		m_SoundEffectMap[name] = new SoundEffect(path);
		return m_SoundEffectMap[name];
	}
}

// Soundstream pre-loading
SoundStream* ResourceManager::GetSoundStream(const std::string& name)
{
	return m_SoundStreamMap.find(name)->second;
}

SoundStream* ResourceManager::LoadSoundStream(const std::string& path, const std::string& name)
{
	auto pSound = m_SoundStreamMap.find(name);
	if(pSound != m_SoundStreamMap.end())
	{
		ERR("SoundStream '" << name << "' already loaded...");
		return pSound->second;
	}
	else
	{
		LOG("Preloading SoundStream: " << name);
		m_SoundStreamMap[name] = new SoundStream(path);
		return m_SoundStreamMap[name];
	}
}

// Text renderer preloading
TextRenderer* ResourceManager::GetTextRenderer(const std::string& name)
{
	return m_TextRenderMap.find(name)->second;
}

TextRenderer* ResourceManager::LoadTextRenderer(const std::string& path, const std::string& name)
{
	auto pTextRender = m_TextRenderMap.find(name);
	
	if(pTextRender != m_TextRenderMap.end())
	{
		ERR("TextRenderer '" << name << "' already loaded...");
		return pTextRender->second;
	}
	else
	{
		LOG("Preloading TextRenderer: " << name);
		m_TextRenderMap[name] = new TextRenderer(path);
		return m_TextRenderMap[name];
	}
}

void ResourceManager::AddName(const std::string name)
{
	m_Names.push_back(name);
}

const std::string& ResourceManager::GetName() const
{
	int i = utils::RandInterval(0, m_Names.size() - 1);
	return m_Names[i];
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
	
	for(std::pair<std::string, SoundStream*> snd : m_SoundStreamMap)
		delete snd.second;
	
	for(std::pair<std::string, TextRenderer*> txt : m_TextRenderMap)
		delete txt.second;
}
