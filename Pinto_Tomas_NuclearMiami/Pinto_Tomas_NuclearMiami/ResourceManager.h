#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>
#include <vector>
#include <map>

#include "utils.h"

class Texture;
class SoundEffect;
class SoundStream;
class TextRenderer;

class ResourceManager
{
public:
	static ResourceManager* Get();
	
	Texture* GetTexture(const std::string& name);
	Texture* LoadTexture(const std::string& path, const std::string& name);
	
	SoundEffect* GetSoundEffect(const std::string& name);
	SoundEffect* LoadSoundEffect(const std::string& path, const std::string& name);
	
	SoundStream* GetSoundStream(const std::string& name);
	SoundStream* LoadSoundStream(const std::string& path, const std::string& name);
	
	TextRenderer* GetTextRenderer(const std::string& name);
	TextRenderer* LoadTextRenderer(const std::string& path, const std::string& name, const Color4f color);

	void AddName(const std::string name);
	const std::string& GetName() const;
	
	~ResourceManager();

private:
	ResourceManager();
	
	ResourceManager(const ResourceManager& other) = delete;
	ResourceManager& operator=(const ResourceManager& other) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;
	
	std::map<std::string, Texture*> m_TextureMap;
	std::map<std::string, SoundEffect*> m_SoundEffectMap;
	std::map<std::string, SoundStream*> m_SoundStreamMap;
	std::map<std::string, TextRenderer*> m_TextRenderMap;
	
	std::vector<std::string> m_Names;
	
	static ResourceManager* m_pTextureManager;
};

#endif // !RESOURCE_MANAGER_H