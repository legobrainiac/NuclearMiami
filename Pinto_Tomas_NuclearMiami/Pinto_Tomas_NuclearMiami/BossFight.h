#ifndef BOSS_FIGHT_H
#define BOSS_FIGHT_H

#include "GameObject.h"

class Texture;
class SoundEffect;

class BossFight :
	public GameObject
{
public:
	BossFight(const Vector2f& position, const Vector2f& scale, float rotation);
	~BossFight();
	
	void Draw() const;
	void Update(float dt);
	
	static int GetBossFightCounter() { return m_BossFightCounter; };
	void SendMessage(MessageType message, int value) override;

private:
	void Shoot(float dt);
	
	void DrawHealth() const;
	void DrawArm() const;
	
	Texture* m_pBossTexture;
	Texture* m_pArmTexture;
	
	SoundEffect* m_pShootingSound;
	
	float m_ArmRotation;
	float m_ShotTimer;
	
	int m_Health;
	
	Vector2f m_CenterPosition;
	
	static int m_BossFightCounter;
};

#endif // !BOSS_FIGHT_H