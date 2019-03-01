#include "pch.h"
#include "GameObject.h"

int GameObject::m_InstanceCounter = 0;

GameObject::GameObject()
: m_Position()
, m_Scale()
, m_Rotation()
{
	++m_InstanceCounter;
}

GameObject::GameObject(const Vector2f& position, const Vector2f& scale, float rotation)
: m_Position(position)
, m_Scale(scale)
, m_Rotation(rotation)
{
	++m_InstanceCounter;
}

// Movement
void GameObject::Translate(const Vector2f& xy)
{
	m_Position += xy;
}

void GameObject::Scale(const Vector2f& xy)
{
	m_Scale += xy;
}

void GameObject::Rotate(float z)
{
	m_Rotation += z;
}

// Getters
const Vector2f& GameObject::GetPosition() const
{
	return m_Position;
}

const Vector2f& GameObject::GetScale() const
{
	return m_Scale;
}

float GameObject::GetRotation() const
{
	return m_Rotation;
}

float GameObject::GetZLayer() const
{
	return m_ZLayer;
}

const std::vector<GameObject*>& GameObject::GetChildren() const
{
	return m_Children;
}

const GameObject* GameObject::GetParent() const
{
	return m_pParent;
}

// Setters
void GameObject::SetPosition(const Vector2f& position)
{
	m_Position = position;
}

void GameObject::SetScale(const Vector2f& scale)
{
	m_Scale = scale;
}

void GameObject::SetRotation(float rotation)
{
	m_Rotation = rotation;
}

void GameObject::SetZLayer(float zLayer)
{
	m_ZLayer = zLayer;
}

void GameObject::AddChild(GameObject* pChild)
{
	m_Children.push_back(pChild);
	pChild->m_pParent = this;
}

void GameObject::Draw() const 
{
	glPushMatrix();
	
	glTranslatef(m_Position.x, m_Position.y, m_ZLayer);
	glRotatef(m_Rotation, 0.f, 0.f, 1.f);
	glScalef(m_Scale.x, m_Scale.y, 0.f);
	
	glColor4f(0.f, 1.f, 0.f, 1.f);
	utils::FillRect(-50.f,-50.f, 100.f, 100.f);
	
	glPopMatrix();
}

void GameObject::Update(float dt) {}
