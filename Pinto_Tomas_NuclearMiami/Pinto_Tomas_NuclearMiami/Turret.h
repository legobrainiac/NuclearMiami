#ifndef TURRET_H
#define TURRET_H

#include "GameObject.h"

class Turret :
	public GameObject
{
public:
	Turret(const Vector2f& position, const Vector2f& scale, float rotation);
	~Turret();
};

#endif // !TURRET_H