#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Vector2f.h"
#include "GameObject.h"

class Projectile : public GameObject
{
public:
	Projectile(const Vector2f& position, const Vector2f& scale, float rotation, const Vector2f& direction, GameObject* shooter);

	void Update(float dt) override;
	void Draw() const override;

protected:
	void Collision(float dt) override;
	
	Vector2f m_Direction;
	float m_Speed;
	
	int m_BounceCount;
	int m_MaxBounceCount;
	
	int m_Dammage;
	
	GameObject* m_pShooter;
};

#endif // !PROJECTILE_H
