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
	AiAgent(const Vector2f& position, const Vector2f& scale, float rotation, GameObject* target, Scene* pScene);
	~AiAgent();
	
	void ChangeTarget(GameObject* pTarget);
	void Update(float dt) override;
	void Draw() const override;
	
	void SendMessage(MessageType message, int value) override;
	
private:
	void DrawBottom() const;
	void DrawTop() const;
	
	GameObject* m_pTarget;
	
	float m_MinDistance;
	float m_MaxDistance;
	float m_MovementSpeed;
	float m_Timer;
	
	Texture* m_pTorsoTexture;
	Sprite* m_pLegsSprite;
	
	int m_Health;
	
	void Ai(float dt);
	void Shoot(Vector2f direction);
};

#endif // !AIAGENT_H