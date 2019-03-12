#ifndef WEAPON_H
#define WEAPON_H

#include "PickUp.h"

class Weapon :
	public PickUp
{
public:
	Weapon();
	~Weapon();
};

#endif // !WEAPON_H