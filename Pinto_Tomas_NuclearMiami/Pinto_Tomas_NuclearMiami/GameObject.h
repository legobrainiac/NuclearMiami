// Tomas Pinto - 2019

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Vector2f.h"
#include <vector>

enum class MessageType
{
	dammage,
	regen
};

class GameObject
{
public:
	GameObject();
	GameObject(const Vector2f& position, const Vector2f& scale, float rotation);
	GameObject(const GameObject& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;

	virtual ~GameObject();
	
	// Movement
	void ApplyForce(const Vector2f& xy);
	// TODO(tomas): public, etc
	void Translate(const Vector2f& xy);
	void Scale(const Vector2f& xy);
	void Rotate(float z);
	
	// Getters
	static int GetInstanceCount() { return m_InstanceCounter; }
	bool IsDirty() const { return m_ShouldDelete; }
	void MakeDirty() { m_ShouldDelete = true; }
	
	const Vector2f& GetPosition() const;
	const Vector2f& GetScale() const;
	float GetRotation() const;
	float GetZLayer() const;
	
	const std::vector<GameObject*>& GetChildren() const;
	const GameObject* GetParent() const;
	std::vector<Point2f> GetCollider();
	
	// Setters
	void SetPosition(const Vector2f& position);
	void SetScale(const Vector2f& scale);
	void SetRotation(float rotation);
	void SetZLayer(float zLayer);
	void SetFriction(float friction);
	
	void AddChild(GameObject* pChild);
	
	// TODO(tomas): template this
	virtual void SendMessage(MessageType message, int value);
	virtual void Draw() const;
	virtual void Update(float dt);
	
protected:
	Vector2f m_Position;
	Vector2f m_Scale;
	Vector2f m_Accelleration;
	
	float m_Rotation;
	float m_Friction;
	float m_ZLayer;
	float m_MaxAcceleration;
	
	GameObject* m_pParent;
	std::vector<GameObject*> m_Children;
	std::vector<Point2f> m_VertexCollider;
	
	bool m_ShouldDelete;
	
	static int m_InstanceCounter;
};

#endif // !GAMEOBJECT_H