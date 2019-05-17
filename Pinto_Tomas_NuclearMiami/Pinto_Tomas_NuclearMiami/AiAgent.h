#ifndef AIAGENT_H
#define AIAGENT_H

#include "GameObject.h"

class Scene;
class Texture;
class Sprite;
class PickUp;
class Weapon;

class AiAgent :
	public GameObject
{
public:
	AiAgent(const Vector2f& position, const Vector2f& scale, float rotation, GameObject* target);
	~AiAgent();
	
	AiAgent(const AiAgent&) = delete;
	AiAgent& operator=(const AiAgent&) = delete;
	AiAgent(AiAgent&&) = delete;
	AiAgent& operator=(AiAgent&&) = delete;
	
	void ChangeTarget(GameObject* pTarget);
	void Update(float dt) override;
	void Draw() const override;
	
	void SendMessage(MessageType message, int value) override;
	bool HasEmptySlot() const override { return m_pWeapons.size() < 1; }
	bool ProcessPickUp(PickUp* pPickUp) override;
	
	static int GetAiInstanceCount() { return m_AiInstanceCounter; }
	
private:
	void DrawBottom() const;
	void DrawTop() const;
	void DrawWeapon() const;
	void DrawHealth() const;
		
	float m_MinDistance;
	float m_MaxDistance;
	float m_MovementSpeed;
	float m_Timer;
	int m_Health;
	
	std::string m_Name;
	Texture* m_pTorsoTexture;
	Sprite* m_pLegsSprite;
	GameObject* m_pTarget;
	std::vector<Weapon*> m_pWeapons;
	
	void Ai(float dt);
	void Shoot(const Vector2f& direction);
	bool InSight(utils::HitInfo& hitOut, Point2f tail, Point2f head);
	
	static int m_AiInstanceCounter;
};

#endif // !AIAGENT_H