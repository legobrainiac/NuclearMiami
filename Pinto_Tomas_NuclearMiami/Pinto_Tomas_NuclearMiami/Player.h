#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"

class Texture;
class Scene;
class Camera;
class PickUp;
struct Window;
struct Circlef;

// TODO(tomas): for now it's a single sprite, eventually this will become a sprite sheet 
class Player : public GameObject
{
public:
	Player(const Vector2f& position, const Vector2f& scale, float rotation, Scene* scene);	
	~Player();
	
	void Draw() const override;
	void Update(float dt) override;
	
	bool HasEmptySlot() const;
	void ProcessPickUp(PickUp* pickUp);
	
private:	
	void Shoot(const Vector2f& direction, float dt);
	void Move(const Uint8* keyStates, float dt);
	
	Texture* m_pTexture;	
	Scene* m_pScene;
	Camera* m_pCamera;
	float m_Timer;
	
	struct WeaponSlots
	{
		GameObject* pPrimary;
		GameObject* pSecondary;
	}m_WeaponSlots;
	
	Circlef m_Collider;
};

#endif // !PLAYER_H

