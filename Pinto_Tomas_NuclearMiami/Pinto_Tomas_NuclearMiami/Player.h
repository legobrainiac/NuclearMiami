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

class Player : public GameObject
{
public:
	Player(const Vector2f& position, const Vector2f& scale, float rotation);	
	Player(const Player& other) = delete;
	Player& operator=(const Player& other) = delete;
	~Player();
	
	void Update(float dt) override;
	void Draw() const override;
	
	void ToggleGodMode() { m_IsGod = !m_IsGod; };
	
	bool HasEmptySlot() const override;
	bool ProcessPickUp(PickUp* pPickUp) override;
	
	bool IsDead() const { return m_Health <= 0; } ;
	void SendMessage(MessageType message, int value) override;
	
	// NOTE(tomas): this should eventually be put in to private, for now since we need it from the outside it stays like this.
	void Drop();
	
private:	
	// Behaviour helpers
	void Shoot(const Vector2f& direction, float dt);
	void Move(const Uint8* pKeyStates, float dt);

	// Draw helpers
	void DrawBottom() const;
	void DrawTop() const;
	void DrawWeapon() const;
	void DrawHealth() const;
	
	int m_Health;
	float m_Timer;
	bool m_IsGod;

	Vector2f m_WeaponPivot;
	
	Texture* m_pWeaponHoldTexture;
	Sprite* m_pLegsSprite;
	Sprite* m_pTorsoSprite;
	Camera* m_pCamera;
	
	std::vector<Weapon*> m_wWeapons;
};

#endif // !PLAYER_H

