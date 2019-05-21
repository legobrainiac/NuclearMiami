#include "pch.h"
#include "GameObject.h"

#include "Matrix2x3.h"
#include "PickUp.h"

int GameObject::m_InstanceCounter = 0;
bool GameObject::m_Debug = false;

GameObject::GameObject(const Vector2f& position, const Vector2f& scale, float rotation)
: m_Position(position)
, m_Scale(scale)
, m_Rotation(rotation)
, m_ZLayer(1.f)
, m_Friction(10.f)
, m_MaxAcceleration(150.f)
, m_CircleCollider(Point2f {}, 8.f)
, m_pScene(Scene::Get())
, m_DoesCollision(true)

{
	++m_InstanceCounter;
}

GameObject::~GameObject()
{
	--m_InstanceCounter;
	
	for(GameObject* pGo : m_pChildren)
		delete pGo;
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

void GameObject::Collision(float dt)
{
	if(m_DoesCollision)
	{
		// Collision with other objects
		for(GameObject* pGo : m_pScene->GetGameObjects())
		{
			if(pGo != this)
			{
				Circlef myCollider = GetCircleCollider();
				Circlef otherCollider = pGo->GetCircleCollider();

				if(myCollider.IsOverlapping(otherCollider) && pGo->m_DoesCollision)
				{
					Vector2f dir { myCollider.center, otherCollider.center };
					float distance = dir.Length();
					float overlapDistance  = (distance - (myCollider.radius + otherCollider.radius)) / 2.f;
			
					m_Position.x -= overlapDistance * (m_Position.x - pGo->m_Position.x) / distance;
					m_Position.y -= overlapDistance * (m_Position.y - pGo->m_Position.y) / distance;
			
					pGo->m_Position.x += overlapDistance * (m_Position.x - pGo->m_Position.x) / distance;
					pGo->m_Position.y += overlapDistance * (m_Position.y - pGo->m_Position.y) / distance;
				}
			}
		}
	}
	
	// Collision with the scene
	GetCircleCollider();
	
	std::vector<Vector2f> hitVertexVector;
	
	for(std::vector<Point2f> collider : m_pScene->GetSceneCollider())
	{
		std::vector<Vector2f> hitVertexTempVector = utils::CustomOverlap(collider, m_CircleCollider);
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
	return m_pChildren;
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

Circlef GameObject::GetCircleCollider()
{
	m_CircleCollider.center = m_Position.ToPoint2f();
	return m_CircleCollider;
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
	m_pChildren.push_back(pChild);
	pChild->m_pParent = this;
}

void GameObject::SetFriction(float friction)
{
	m_Friction = friction;
}

void GameObject::Draw() const 
{
	for(GameObject* pGo : m_pChildren)
		pGo->Draw();
	
	if(m_Debug)
	{
		TextRenderConfig textConfig;
		textConfig.spacing = 3.f;
		textConfig.scale = 0.2f;
		
		// TODO(tomas): maybe dont use get every frame, just saying
		ResourceManager::Get()->GetTextRenderer("munroDebug")->DrawString(m_Position.ToString(), Vector2f {-100.f, 50.f}, textConfig);	
	}	
}

void GameObject::Update(float dt) 
{
	m_PreviousPos = m_Position;
	m_Accelleration = utils::Lerp(m_Accelleration, Vector2f {0.f, 0.f}, dt * m_Friction);
	
	m_Accelleration.x = utils::Clamp(-m_MaxAcceleration, m_MaxAcceleration, m_Accelleration.x);
	
	m_Accelleration.y = utils::Clamp(-m_MaxAcceleration, m_MaxAcceleration, m_Accelleration.y);
	
	Translate(m_Accelleration * dt);
	
	for(GameObject* pGo : m_pChildren)
		pGo->Update(dt);
	
	Collision(dt);
}

void GameObject::SendMessage(MessageType message, int value)
{
}

// These are to be overriden by objects that are suppose to have inventories
bool GameObject::ProcessPickUp(PickUp* pPickup)
{
	return false;
}

bool GameObject::HasEmptySlot() const
{
	return false;
}