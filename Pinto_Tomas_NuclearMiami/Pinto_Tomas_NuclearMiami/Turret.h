#ifndef TURRET_H
#define TURRET_H

#include "GameObject.h"

#define BULLET_INTERVAL .5f

class SoundEffect;

class Turret :
	public GameObject
{
public:
	Turret(const Vector2f& position, const Vector2f& scale, float rotation);
	~Turret();
	
	void Draw() const;
	void Update(float dtn);
	
private:
	bool InSight(Point2f tail, Point2f head);
	
	float m_ShotTimer;
	SoundEffect* m_pShootingSound;
};

#endif // !TURRET_H