#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Vector2f.h"
#include "GameObject.h"

class Projectile : public GameObject
{
public:
	Projectile(const Vector2f& position, const Vector2f& scale, float rotation, const Vector2f& direction, GameObject* shooter);
	Projectile(const Projectile& other) = delete;
	Projectile& operator=(const Projectile& other) = delete;

	void Update(float dt) override;
	void Draw() const override;

private:
	void Collision() override;
	
	Vector2f m_Direction;
	float m_Speed;
	
	int m_BounceCount;
	int m_MaxBounceCount;
	
	GameObject* m_Shooter;
};

#endif // !PROJECTILE_H