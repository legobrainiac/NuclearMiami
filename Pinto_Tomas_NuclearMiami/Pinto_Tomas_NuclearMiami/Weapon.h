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
	Weapon(const Vector2f& position, const Vector2f& scale, float rotation, const std::string& texture, float kickBack, int fireRate);
	Weapon(const Weapon& other) = delete;
	Weapon& operator=(const Weapon& other) = delete;
	~Weapon();
	
	void Draw() const;
	void Update(float dt);
	virtual void Shoot(const Vector2f& position, const Vector2f& direction, float rofMod = 1.f);
	
	void SetInWorld(bool val) override;
	
	virtual int GetAmmo() const { return m_AmmoCount; }
	virtual void AddAmmo(int ammotCount) { m_AmmoCount += ammotCount; }
	
protected:
	Texture* m_pTexture;
	SoundEffect* m_pShootingSound;
	SoundEffect* m_pEquip;
	SoundEffect* m_pDrop;
	
	float m_RotationToPlayer;
	float m_DistanceToPlayer;
	float m_KickBack;
	float m_Timer;
	int m_RateOfFire;
	
	int m_AmmoCount;
};

#endif // !WEAPON_H