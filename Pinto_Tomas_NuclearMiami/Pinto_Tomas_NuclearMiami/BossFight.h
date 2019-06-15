#ifndef BOSS_FIGHT_H
#define BOSS_FIGHT_H

#include "GameObject.h"

class BossFight :
	public GameObject
{
public:
	BossFight(const Vector2f& position, const Vector2f& scale, float rotation);
	~BossFight();
	
	void Draw() const;
	void Update(float dt);
	
	static int GetBossFightCounter() { return m_BossFightCounter; };

private:
	Texture* m_pBossTexture;
	Texture* m_pArmTexture;
	
	float m_ArmRotation;
	
	static int m_BossFightCounter;
};

#endif // !BOSS_FIGHT_H