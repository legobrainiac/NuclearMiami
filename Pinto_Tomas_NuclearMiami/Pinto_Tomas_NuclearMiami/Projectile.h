#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Vector2f.h"
#include "GameObject.h"

class Projectile : public GameObject
{
public:
	Projectile(const Vector2f& position, const Vector2f& scale, float rotation, const Vector2f& direction);

	void Update(float dt) override;
	void Draw() const override;
private:
	Vector2f m_Direction;
	float m_Speed;
};

#endif // !PROJECTILE_H