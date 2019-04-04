#ifndef WEAPON_H
#define WEAPON_H

#include "PickUp.h"

class Player;
class GameObject;
class Texture;
class Scene;

class Weapon :
	public PickUp
{
public:
	Weapon(const Vector2f& position, const Vector2f& scale, float rotation, GameObject* pOwner, const std::string& texturePath, float kickBack);
	~Weapon();
	
	void Draw() const;
	void Update(float dt);
	void Shoot(const Vector2f& position, const Vector2f& direction, Scene* pScene);
	
private:
	Texture* m_pTexture;
	float m_KickBack;
	float m_Timer;
	int m_RateOfFire;
};

#endif // !WEAPON_H