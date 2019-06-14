#ifndef BOSS_FIGHT_H
#define BOSS_FIGHT_H

#include "GameObject.h"

class BossFight :
	public GameObject
{
public:
	BossFight(const Vector2f& position, const Vector2f& scale, float rotation);
	~BossFight();
};

#endif // !BOSS_FIGHT_H