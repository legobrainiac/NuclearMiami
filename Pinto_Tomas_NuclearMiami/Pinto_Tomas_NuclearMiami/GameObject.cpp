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

GameObject::~GameObject()
{
	for(auto go : m_Children)
		delete go;
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
	for(auto go : m_Children)
		go->Draw();
}

void GameObject::Update(float dt) 
{
	for(auto go : m_Children)
		go->Update(dt);
}