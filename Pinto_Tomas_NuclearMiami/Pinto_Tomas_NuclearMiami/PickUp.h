#ifndef PICKUP_H
#define PICKUP_H

#include "GameObject.h"

class PickUp :
	public GameObject
{
public:
	PickUp(const Vector2f& position, const Vector2f& scale, float rotation);
	~PickUp();
	
	void Update(float dt);
	void Draw() const;
};

#endif // !PICKUP_H