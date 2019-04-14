#ifndef WEAPON_H
#define WEAPON_H

#include "PickUp.h"
#include "ResourceManager.h"

class Player;
class GameObject;
class Texture;
class Scene;
class SoundEffect;

class Weapon :
	public PickUp
{
public:
	Weapon(const Vector2f& position, const Vector2f& scale, float rotation, GameObject* pOwner, const std::string& texture, float kickBack, int fireRate);
	Weapon(const Weapon& other) = delete;
	Weapon& operator=(const Weapon& other) = delete;
	~Weapon();
	
	void Draw() const;
	void Update(float dt);
	void Shoot(const Vector2f& position, const Vector2f& direction, Scene* pScene);
	
	void SetInWorld(bool val) override;
private:
	Texture* m_pTexture;
	SoundEffect* m_pShootingSound;
	SoundEffect* m_pEquip;
	SoundEffect* m_pDrop;
	float m_KickBack;
	float m_Timer;
	int m_RateOfFire;
};

#endif // !WEAPON_H