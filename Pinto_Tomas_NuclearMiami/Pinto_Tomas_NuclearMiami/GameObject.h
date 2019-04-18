// Tomas Pinto - 2019

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Vector2f.h"
#include <vector>

class Scene;
class PickUp;
struct Circlef;

enum class MessageType
{
	dammage,
	regen
};

class IDynamicObject
{
public:
	virtual void Draw() const = 0;
	virtual void Update(float dt) = 0;
};

class IInventory
{
public:
	virtual bool ProcessPickUp(PickUp* pickup) = 0;
	virtual bool HasEmptySlot() const = 0;
};

class GameObject : public IDynamicObject, public IInventory
{
public:
	GameObject(const Vector2f& position, const Vector2f& scale, float rotation);	
	GameObject(const GameObject&) = delete;
	GameObject& operator=(const GameObject&) = delete;
	virtual ~GameObject();
	
	void Draw() const override;
	void Update(float dt) override;
	
	// GameObject position
	const Vector2f& GetPosition() const;
	void SetPosition(const Vector2f& position);
	
	// GameObject scale
	const Vector2f& GetScale() const;
	void SetScale(const Vector2f& scale);
	
	// GameObject rotation
	float GetRotation() const;
	void SetRotation(float rotation);
	
	// GameObject z-sorting layer, render order
	float GetZLayer() const;
	void SetZLayer(float zLayer);
	
	// GameObject children and parent
	void AddChild(GameObject* pChild);
	const std::vector<GameObject*>& GetChildren() const;
	const GameObject* GetParent() const;
	
	// Pickup handling
	bool ProcessPickUp(PickUp* pickup) override;
	bool HasEmptySlot() const override;
	
	std::vector<Point2f> GetCollider();
	Circlef GetCircleCollider();
	void SetFriction(float friction);
	void ApplyForce(const Vector2f& xy);

	// Message system to avoid Runtime type checking for everything
	// TODO(tomas): template this
	virtual void SendMessage(MessageType message, int value);
	
	// Object cleanup helpers
	bool IsDirty() const { return m_ShouldDelete; }
	virtual void MakeDirty() { m_ShouldDelete = true; }
	
	static int GetInstanceCount() { return m_InstanceCounter; }
	
protected:
	void Translate(const Vector2f& xy);
	void Scale(const Vector2f& xy);
	void Rotate(float z);
	
	virtual void Collision(float dt);
	
	Vector2f m_Position;
	Vector2f m_Scale;
	Vector2f m_Accelleration;
	Vector2f m_PreviousPos;
	Circlef m_CircleCollider;
	
	float m_Rotation;
	float m_Friction;
	float m_ZLayer;
	float m_MaxAcceleration;
	bool m_ShouldDelete;
	bool m_DoesCollision;
	
	GameObject* m_pParent;
	Scene* m_pScene;
	
	std::vector<GameObject*> m_Children;
	std::vector<Point2f> m_VertexCollider;
	
	static int m_InstanceCounter;
};

#endif // !GAMEOBJECT_H