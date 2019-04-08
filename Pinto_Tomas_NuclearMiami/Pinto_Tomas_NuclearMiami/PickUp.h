#ifndef PICKUP_H
#define PICKUP_H

#include "GameObject.h"

class Scene;

class PickUp :
	public GameObject
{
public:
	PickUp(const Vector2f& position, const Vector2f& scale, float rotation, GameObject* pOwner, Scene* pScene);
	~PickUp();
	
	void Update(float dt);
	void Draw() const;
	
	void SetInWorld(bool val) { m_InWorld = val; }
	
protected:
	GameObject* m_pOwner;
	bool m_InWorld;
};

#endif // !PICKUP_H