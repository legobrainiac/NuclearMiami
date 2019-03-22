#ifndef PICKUP_H
#define PICKUP_H

#include "GameObject.h"

class Player;

class PickUp :
	public GameObject
{
public:
	PickUp(const Vector2f& position, const Vector2f& scale, float rotation, Player* player);
	~PickUp();
	
	void Update(float dt);
	void Draw() const;
	
private:
	Player* m_pPlayer;
};

#endif // !PICKUP_H