#ifndef AIAGENT_H
#define AIAGENT_H

#include "GameObject.h"

class Scene;
class Texture;
class Sprite;

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
	
private:
	void DrawBottom() const;
	void DrawTop() const;
	void DrawHealth() const;
	
	GameObject* m_pTarget;
	
	float m_MinDistance;
	float m_MaxDistance;
	float m_MovementSpeed;
	float m_Timer;
	
	Texture* m_pTorsoTexture;
	Sprite* m_pLegsSprite;
	
	int m_Health;
	
	bool InSight(utils::HitInfo& hitOut, Point2f tail, Point2f head);
	
	void Ai(float dt);
	void Shoot(Vector2f direction);
};

#endif // !AIAGENT_H