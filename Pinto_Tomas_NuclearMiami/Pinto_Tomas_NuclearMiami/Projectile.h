#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Vector2f.h"
#include "GameObject.h"

class Scene;

class Projectile : public GameObject
{
public:
	Projectile(const Vector2f& position, const Vector2f& scale, float rotation, const Vector2f& direction, Scene* pScene);

	void Update(float dt) override;
	void Draw() const override;

private:
	void Collision();
	
	Scene* m_pScene;
	Vector2f m_Direction;
	float m_Speed;
	
	int m_BounceCount;
};

#endif // !PROJECTILE_H