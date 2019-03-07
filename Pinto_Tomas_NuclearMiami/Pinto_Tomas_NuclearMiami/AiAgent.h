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
	
	void ChangeTarget(GameObject* target);
	void Update(float dt) override;
	void Draw() const override;
	
private:
	GameObject* m_pTarget;
	Scene* m_pScene;
	
	float m_MinDistance;
	float m_MaxDistance;
	float m_MovementSpeed;
	
	void Ai(float dt);
};

#endif // !AIAGENT_H