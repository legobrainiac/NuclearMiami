#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "GameObject.h"

class Sprite;
class SoundEffect;

class Explosion :
	public GameObject
{
public:
	Explosion(const Vector2f& position);
	~Explosion();
	
	void Draw() const;
	void Update(float dt);
	
protected:
	Sprite* m_pExplosion;
	SoundEffect* m_pExplosionSound;
	float m_DeleteTimer;
};

#endif // !EXPLOSION_H
