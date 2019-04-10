#include "pch.h"
#include "GameObject.h"
#include "Matrix2x3.h"
#include "structs.h"
#include "Scene.h"

int GameObject::m_InstanceCounter = 0;

GameObject::GameObject()
: m_Position()
, m_Scale()
, m_Rotation()
, m_ZLayer(1.f)
, m_Friction(10.f)
, m_MaxAcceleration(150.f)
{
	++m_InstanceCounter;
}

GameObject::GameObject(const Vector2f& position, const Vector2f& scale, float rotation, Scene* pScene)
: m_Position(position)
, m_Scale(scale)
, m_Rotation(rotation)
, m_ZLayer(1.f)
, m_Friction(10.f)
, m_MaxAcceleration(150.f)
, m_CircleCollider(Point2f {}, 10.f)
, m_pScene(pScene)

{
	++m_InstanceCounter;
	m_VertexCollider.push_back(Point2f {-10.f, -5.f});
	m_VertexCollider.push_back(Point2f {-10.f, 5.f});
	m_VertexCollider.push_back(Point2f {10.f, 5.f});
	m_VertexCollider.push_back(Point2f {10.f, -5.f});
}

GameObject::~GameObject()
{
	--m_InstanceCounter;
	for(GameObject* go : m_Children)
		delete go;
}

// Movement
void GameObject::ApplyForce(const Vector2f& xy)
{
	m_Accelleration += xy;
}

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

void GameObject::Collision()
{
	// Collision test
	m_CircleCollider.center = m_Position.ToPoint2f();
	
	std::vector<Vector2f> hitVertexVector;
	
	for(std::vector<Point2f> collider : m_pScene->GetSceneCollider())
	{
		std::vector<Vector2f> hitVertexTempVector = utils::CustomOverlap(collider, m_CircleCollider);
		
		// NOTE(tomas): this is not good xD, good for now but not in the long run
		hitVertexVector.insert( hitVertexVector.end(), hitVertexTempVector.begin(), hitVertexTempVector.end() );
	}

	Vector2f final {};

	if(hitVertexVector.size() > 1)
	{
		Vector2f medianNormal {};
		Vector2f backtrack{ m_Position.ToPoint2f(), m_PreviousPos.ToPoint2f() };		
		
		hitVertexVector.pop_back();
		for(Vector2f hit : hitVertexVector)
			medianNormal += hit.Orthogonal().Normalized();


		medianNormal = medianNormal.Normalized();

		final = (medianNormal * backtrack.Length());
	}
	else
	{
		for(Vector2f hit : hitVertexVector)
		{
			Vector2f normal = Vector2f { std::abs(hit.y), std::abs(hit.x) }.Normalized();
			Vector2f backtrack{ m_Position.ToPoint2f(), m_PreviousPos.ToPoint2f() };		
			
			final.x = (backtrack.x * normal.x);
			final.y = (backtrack.y * normal.y);
		}
	}
	
	Translate(final);
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

// Returns world space collider for this object
std::vector<Point2f> GameObject::GetCollider()
{
	Matrix2x3 tMat = Matrix2x3::CreateTranslationMatrix(m_Position.x, m_Position.y);
	Matrix2x3 rMat = Matrix2x3::CreateRotationMatrix(m_Rotation);
	
	return tMat.Transform(rMat.Transform(m_VertexCollider));
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

void GameObject::SetFriction(float friction)
{
	m_Friction = friction;
}

void GameObject::Draw() const 
{
	for(GameObject* go : m_Children)
		go->Draw();
}

void GameObject::Update(float dt) 
{
	m_PreviousPos = m_Position;
	m_Accelleration = utils::Lerp(m_Accelleration, Vector2f {0.f, 0.f}, dt * m_Friction);
	
	m_Accelleration.x = utils::Clamp(-m_MaxAcceleration, m_MaxAcceleration, m_Accelleration.x);
	
	m_Accelleration.y = utils::Clamp(-m_MaxAcceleration, m_MaxAcceleration, m_Accelleration.y);
	
	Translate(m_Accelleration * dt);
	
	for(GameObject* go : m_Children)
		go->Update(dt);
	
	Collision();
}

void GameObject::SendMessage(MessageType message, int value)
{
}