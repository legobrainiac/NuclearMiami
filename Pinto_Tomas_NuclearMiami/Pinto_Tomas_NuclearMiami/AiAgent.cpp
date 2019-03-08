#include "pch.h"
#include "AiAgent.h"

#include "utils.h"
#include "Scene.h"

AiAgent::AiAgent(const Vector2f& position, const Vector2f& scale, float rotation, GameObject* pTarget, Scene* scene)
: GameObject(position, scale, rotation)
, m_pTarget(pTarget)
, m_pScene(scene)
, m_MinDistance(30.f)
, m_MaxDistance(1000.f)
, m_MovementSpeed(100.f)
{
}

AiAgent::~AiAgent()
{
}

void AiAgent::ChangeTarget(GameObject* pTarget)
{
	m_pTarget = pTarget;
}

void AiAgent::Update(float dt)
{
	Ai(dt);
}

void AiAgent::Draw() const
{
	glPushMatrix();
	
	glTranslatef(m_Position.x, m_Position.y, m_ZLayer);
	glRotatef(m_Rotation, 0.f, 0.f, 1.f);
	glScalef(m_Scale.x, m_Scale.y, 0.f);
	
	utils::FillEllipse(0.f, 0.f , 5.f, 5.f);
	
	GameObject::Draw();
	
	glPopMatrix();
}

void AiAgent::Ai(float dt)
{
	utils::HitInfo hit;
	
	Vector2f ray{ m_Position.ToPoint2f(), m_pTarget->GetPosition().ToPoint2f() };
	
	// Cast first ray, if target in sight, move to it
	if(!Raycast(m_pScene->GetSceneCollider(), m_Position.ToPoint2f(), m_pTarget->GetPosition().ToPoint2f(), hit))
	{
		if(ray.Length() > m_MinDistance)
			Translate(ray.Normalized() * m_MovementSpeed * dt);
	}
	else // If target not in sight, attempt bounce path-find
	{
		utils::HitInfo hitBounce;
		Vector2f reflection = ray.Reflect(hit.normal);
		
		// First bounce
		if(Raycast(m_pScene->GetSceneCollider(), hit.intersectPoint, (reflection.Normalized() * m_MaxDistance).ToPoint2f(), hitBounce))
		{
			utils::HitInfo hitFinal;
			
			// Last Bounce
			if(!Raycast(m_pScene->GetSceneCollider(), hitBounce.intersectPoint, m_pTarget->GetPosition().ToPoint2f(), hitFinal))
			{
				// From this point if no obstacle is in sight, move to point
				float tX = (hit.intersectPoint.x + hitBounce.intersectPoint.x) / 2.f;
				float tY = (hit.intersectPoint.y + hitBounce.intersectPoint.y) / 2.f;
				
				Point2f targetTemp {tX, tY};
				Vector2f direction {m_Position.ToPoint2f(), Point2f{ tX, tY }};
				
				Translate(direction.Normalized() * m_MovementSpeed * dt);
			}
		}	
	}
}