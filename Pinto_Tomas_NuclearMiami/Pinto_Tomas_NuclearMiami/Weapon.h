#ifndef WEAPON_H
#define WEAPON_H

#include "PickUp.h"

class Player;

class Weapon :
	public PickUp
{
public:
	Weapon(const Vector2f& position, const Vector2f& scale, float rotation, Player* player);
	~Weapon();
};

#endif // !WEAPON_H