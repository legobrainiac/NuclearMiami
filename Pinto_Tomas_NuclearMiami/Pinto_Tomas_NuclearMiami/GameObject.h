// Tomas Pinto - 2019

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Vector2f.h"
#include <vector>

class GameObject
{
public:
	GameObject();
	GameObject(const Vector2f& position, const Vector2f& scale, float rotation);
	
	virtual ~GameObject();
	
	// Movement
	void Translate(const Vector2f& xy);
	void Scale(const Vector2f& xy);
	void Rotate(float z);
	
	// Getters
	static int GetInstanceCount() { return m_InstanceCounter; }
	
	const Vector2f& GetPosition() const;
	const Vector2f& GetScale() const;
	float GetRotation() const;
	float GetZLayer() const;
	
	const std::vector<GameObject*>& GetChildren() const;
	const GameObject* GetParent() const;
	
	// Setters
	void SetPosition(const Vector2f& position);
	void SetScale(const Vector2f& scale);
	void SetRotation(float rotation);
	void SetZLayer(float zLayer);
	
	void AddChild(GameObject* child);
	
	// Pure virtuals
	virtual void Draw() const;
	virtual void Update(float dt);
	
protected:
	Vector2f m_Position;
	Vector2f m_Scale;
	float m_Rotation;
	float m_ZLayer;
	
	GameObject* m_pParent;
	std::vector<GameObject*> m_Children;
	
	static int m_InstanceCounter;
};

#endif // !GAMEOBJECT_H