#ifndef ROCKET_LAUNCHER_H
#define ROCKET_LAUNCHER_H

#include "Weapon.h"

class RocketLauncher :
	public Weapon
{
public:
	RocketLauncher(const Vector2f& position, const Vector2f& scale, float rotation);
	RocketLauncher(const Weapon& other) = delete;
	RocketLauncher& operator=(const Weapon& other) = delete;
	~RocketLauncher();
	
	void Shoot(const Vector2f& position, const Vector2f& direction, float rofMod = 1.f);
};

#endif // !ROCKET_LAUNCHER_H
