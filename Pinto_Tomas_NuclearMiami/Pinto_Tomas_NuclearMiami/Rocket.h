#ifndef ROCKET_H
#define ROCKET_H

#include "Projectile.h"

class Texture;

class Rocket :
	public Projectile
{
public:
	Rocket(const Vector2f& position, const Vector2f& scale, float rotation, const Vector2f& direction, GameObject* shooter);
	~Rocket();

	void Draw() const;
	void Collision(float dt) override;
	
private:
	void DoHit();
	
	Texture* m_pTexture;
	float m_Radius;
};

#endif // !ROCKET_H
