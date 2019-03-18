#ifndef AIAGENT_H
#define AIAGENT_H

#include "GameObject.h"

class Scene;

class AiAgent :
	public GameObject
{
public:
	AiAgent(const Vector2f& position, const Vector2f& scale, float rotation, GameObject* target, Scene* scene);
	~AiAgent();
	
	void ChangeTarget(GameObject* pTarget);
	void Update(float dt) override;
	void Draw() const override;
	
private:
	GameObject* m_pTarget;
	Scene* m_pScene;
	
	float m_MinDistance;
	float m_MaxDistance;
	float m_MovementSpeed;
	float m_Timer;
	
	void Ai(float dt);
	void Shoot(Vector2f direction);
};

#endif // !AIAGENT_H