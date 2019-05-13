#ifndef HEALTHPICKUP_H
#define HEALTHPICKUP_H

#include "PickUp.h"

class Texture;

class HealthPickup :
	public PickUp
{
public:
	HealthPickup(const Vector2f& position, const Vector2f& scale, float rotation, int healthGain);

	void Draw() const;
	int GetHealthGain();
	
private:
	int m_HealthGain;
	Texture* m_pTexture;
};

#endif // !HEALTHPICKUP_H