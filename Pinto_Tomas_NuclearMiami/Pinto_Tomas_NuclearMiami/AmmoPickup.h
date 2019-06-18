#ifndef AMMO_PICKUP_H
#define AMMO_PICKUP_H

#include "PickUp.h"

class AmmoPickup :
	public PickUp
{
public:
	AmmoPickup(const Vector2f& position, const Vector2f& scale, float rotation, int ammoGain);
	
	void Draw() const;
	int GetAmmoGain();
	
private:
	int m_AmmoGain;
	Texture* m_pTexture;
};

#endif // !AMMO_PICKUP_H