#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Vector2f.h"
#include "GameObject.h"

class Scene;

class Projectile : public GameObject
{
public:
	Projectile(const Vector2f& position, const Vector2f& scale, float rotation, const Vector2f& direction, Scene* pScene, GameObject* shooter);

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