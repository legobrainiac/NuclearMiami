#include "pch.h"
#include "Weapon.h"
#include "Player.h"

Weapon::Weapon(const Vector2f& position, const Vector2f& scale, float rotation, Player* player)
: PickUp(position, scale, rotation, player)
{
}


Weapon::~Weapon()
{
}
