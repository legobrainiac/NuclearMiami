#ifndef WEAPON_H
#define WEAPON_H

#include "PickUp.h"

class Weapon :
	public PickUp
{
public:
	Weapon(const Vector2f& position, const Vector2f& scale, float rotation);
	~Weapon();
};

#endif // !WEAPON_H