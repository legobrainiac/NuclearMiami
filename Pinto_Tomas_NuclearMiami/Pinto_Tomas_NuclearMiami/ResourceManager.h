#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>
#include <map>

#include "utils.h"

class Texture;
class SoundEffect;
class SoundStream;

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
	
	~ResourceManager();

private:
	ResourceManager();
	
	std::map<std::string, Texture*> m_TextureMap;
	std::map<std::string, SoundEffect*> m_SoundEffectMap;
	std::map<std::string, SoundStream*> m_SoundStreamMap;
	static ResourceManager* m_pTextureManager;
};

#endif // !RESOURCE_MANAGER_H