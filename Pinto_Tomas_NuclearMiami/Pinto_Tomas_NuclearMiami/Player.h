#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"

class Texture;
class Sprite;
class Scene;
class Camera;
class PickUp;
class Weapon;
struct Window;
struct Circlef;

// TODO(tomas): for now it's a single sprite, eventually this will become a sprite sheet 
class Player : public GameObject
{
public:
	Player(const Vector2f& position, const Vector2f& scale, float rotation, Scene* pScene);	
	~Player();
	
	void Draw() const override;
	void Update(float dt) override;
	
	bool HasEmptySlot() const;
	void ProcessPickUp(GameObject* pickUp);
	
	void SendMessage(MessageType message, int value) override;
	
private:	
	void Shoot(const Vector2f& direction, float dt);
	void Move(const Uint8* keyStates, float dt);
	
	void DrawBottom() const;
	void DrawTop() const;
	void DrawWeapon() const;
	void DrawHealth() const;
	
	int m_Health;
	float m_Timer;

	Vector2f m_WeaponPivot;
	
	Texture* m_pTorsoTexture;
	Sprite* m_pLegsSprite;
	Camera* m_pCamera;
	
	std::vector<Weapon*> m_Weapons;
};

#endif // !PLAYER_H

