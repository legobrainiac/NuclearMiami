#ifndef PICKUP_H
#define PICKUP_H

#include "GameObject.h"

class Scene;

class PickUp :
	public GameObject
{
public:
	PickUp(const Vector2f& position, const Vector2f& scale, float rotation);
	PickUp(const PickUp& other) = delete;
	PickUp& operator=(const PickUp& other) = delete;
	~PickUp();
	
	void Update(float dt);
	void Draw() const;
	
	virtual void SetInWorld(bool val);
	void SetOwner(GameObject* pOwner) { m_pOwner = pOwner; }
	
protected:
	GameObject* m_pOwner;
	float m_PickupTimer;
	bool m_InWorld;
};

#endif // !PICKUP_H