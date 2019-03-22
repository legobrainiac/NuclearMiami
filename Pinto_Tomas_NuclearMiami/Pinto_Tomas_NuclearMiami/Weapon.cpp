#include "pch.h"
#include "Weapon.h"


Weapon::Weapon(const Vector2f& position, const Vector2f& scale, float rotation)
: PickUp(position, scale, rotation)
{
}


Weapon::~Weapon()
{
}
